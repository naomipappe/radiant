#pragma once

#include "fmt/base.h"

#include <limits>
#include <numeric>
#include <optional>
#include <span>
#include <sys/types.h>
#include <vector>

#include <core/types.hpp>
#include <core/math.hpp>
#include <core/primitive.hpp>
#include <core/acceleration_structures/aggregate.hpp>

namespace radiant
{

struct BVHNode
{
    vec3 aabb_min, aabb_max;
    u32  left_child          = 0;
    u32  first_primitive_idx = 0;
    u32  primitive_count     = 0;
};

class BVHAggregate : public Aggregate
{
  public:
    explicit BVHAggregate(std::span<GeometricPrimitive*> primitives) : m_primitives(primitives.begin(), primitives.end())
    {
        build(primitives);
        m_dirty = false;
    }

    BVHAggregate()           = default;
    ~BVHAggregate() override = default;

    void insert(GeometricPrimitive* primitive) override
    {
        m_primitives.push_back(primitive);
        m_dirty = true;
    }

    [[nodiscard]] bool test_intersection(const Ray& r, Scalar tmin, Scalar tmax) const override
    {
        return intersect(r, tmin, tmax) != std::nullopt;
    }

    [[nodiscard]] std::optional<SurfaceIntersection> intersect(const Ray& r, Scalar tmin, Scalar tmax) const override
    {
        return intersect(r, tmin, tmax, 0);
    }

    void clear() override {};

    void build()
    {
        m_nodes.clear();
        build(m_primitives);
        m_dirty = false;
    }

  protected:
    void build(const std::span<GeometricPrimitive*> primitives)
    {
        m_nodes.reserve(2 * primitives.size() - 1);
        m_primitive_indices.resize(m_primitives.size());
        std::iota(m_primitive_indices.begin(), m_primitive_indices.end(), 0);
        BVHNode& root            = m_nodes.emplace_back();
        root.first_primitive_idx = 0;
        root.primitive_count     = primitives.size();

        update_bounds(0);
        subdivide(0);
    }

    void update_bounds(u32 node_idx)
    {
        BVHNode& node = m_nodes[node_idx];
        node.aabb_min = vec3(std::numeric_limits<Scalar>::max());
        node.aabb_max = vec3(std::numeric_limits<Scalar>::lowest());
        for (u32 offset = node.first_primitive_idx, p = 0; p < node.primitive_count; ++p)
        {
            u32         tri_idx   = m_primitive_indices[offset + p];
            const auto& primitive = m_primitives[tri_idx];
            node.aabb_min         = pointwise_min(node.aabb_min, primitive->aabb_min());
            node.aabb_max         = pointwise_max(node.aabb_max, primitive->aabb_max());
        }
    }

    void subdivide(u32 node_idx)
    {
        BVHNode& node = m_nodes[node_idx];

        if (node.primitive_count <= 2)
        {
            return;
        }

        vec3 extent = node.aabb_max - node.aabb_min;
        u32  axis   = 0;
        if (extent[1] > extent[axis])
        {
            axis = 1;
        }
        if (extent[2] > extent[axis])
        {
            axis = 2;
        }
        const Scalar split_pos = node.aabb_min[axis] + extent[axis] * 0.5f;

        u32 i = node.first_primitive_idx;
        u32 j = i + node.primitive_count - 1;
        while (i <= j)
        {
            if (m_primitives[m_primitive_indices[i]]->m_shape->m_centroid[axis] < split_pos)
            {
                i++;
            }
            else
            {
                std::swap(m_primitive_indices[i], m_primitive_indices[j--]);
            }
        }

        const u32 left_count = i - node.first_primitive_idx;
        if (left_count == 0 || left_count == node.primitive_count)
        {
            return;
        }

        const u32 left_child          = m_nodes.size();
        node.left_child               = left_child;
        BVHNode& left_node            = m_nodes.emplace_back(BVHNode{});
        left_node.first_primitive_idx = node.first_primitive_idx;
        left_node.primitive_count     = left_count;

        const u32 right_child          = m_nodes.size();
        BVHNode&  right_node           = m_nodes.emplace_back(BVHNode{});
        right_node.first_primitive_idx = i;
        right_node.primitive_count     = node.primitive_count - left_count;

        node.primitive_count = 0;

        update_bounds(left_child);
        update_bounds(right_child);

        subdivide(left_child);
        subdivide(right_child);
    }

    [[nodiscard]] std::optional<SurfaceIntersection> intersect(const Ray& r, Scalar tmin, Scalar tmax, u32 node_idx) const
    {
        const BVHNode& node = m_nodes[node_idx];
        if (!test_intersection_aabb(r, node.aabb_min, node.aabb_max))
        {
            return std::nullopt;
        }

        if (node.primitive_count != 0)
        {
            std::optional<SurfaceIntersection> closest;
            Scalar                             curtmax = tmax;

            for (u32 idx = node.first_primitive_idx; idx < node.primitive_count + node.first_primitive_idx; ++idx)
            {
                debug_intersection_tests++;
                std::optional<SurfaceIntersection> intersection =
                    m_primitives[m_primitive_indices[idx]]->intersect(r, tmin, curtmax);
                if (intersection)
                {
                    if (!closest || intersection->m_t < closest->m_t)
                    {
                        closest = intersection;
                        curtmax = closest->m_t;
                    }
                }
            }
            return closest;
        }
        std::optional<SurfaceIntersection> closest;
        Scalar                             curtmax = tmax;
        if (const auto intersection = intersect(r, tmin, tmax, node.left_child); intersection)
        {
            closest = intersection;
            curtmax = intersection->m_t;
        }

        if (const auto intersection = intersect(r, tmin, curtmax, node.left_child + 1); intersection)
        {
            if (!closest || intersection->m_t < closest->m_t)
            {
                closest = intersection;
            }
        }
        return closest;
    }

  protected:
    bool                             m_dirty{ true };
    std::vector<BVHNode>             m_nodes;
    std::vector<GeometricPrimitive*> m_primitives;
    std::vector<u32>                 m_primitive_indices;

  public:
    mutable u32 debug_intersection_tests = 0;
};

}; // namespace radiant
