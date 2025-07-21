#pragma once

#include <limits>
#include <memory>
#include <span>
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
    u32  left_child{ 0 }, right_child{ 0 };
    bool leaf{ false };
    u32  first_primitive{ 0 }, primitive_count{ 0 };
};
class BVHAggregate : public Aggregate
{
  public:
    explicit BVHAggregate(std::span<std::shared_ptr<GeometricPrimitive>> primitives) :
        m_primitives(primitives.begin(), primitives.end())
    {
        build(primitives);
        m_dirty = false;
    }

    BVHAggregate()                   = default;
    virtual ~BVHAggregate() override = default;

    void insert(std::shared_ptr<GeometricPrimitive> primitive)
    {
        m_primitives.push_back(primitive);
        m_dirty = true;
    }

    void build()
    {
        m_nodes.clear();
        build(m_primitives);
        m_dirty = false;
    }

  protected:
    void build(std::span<std::shared_ptr<GeometricPrimitive>> primitives)
    {
        m_nodes.reserve(2 * primitives.size() - 1);
        BVHNode& root        = m_nodes.emplace_back();
        root.first_primitive = 0;
        root.primitive_count = primitives.size();
    }

    void update_bounds(u32 node_idx)
    {
        BVHNode& node = m_nodes[node_idx];
        node.aabb_min = vec3(std::numeric_limits<Scalar>::max());
        node.aabb_max = vec3(-std::numeric_limits<Scalar>::max());
        for (u32 offset = node.first_primitive, p = 0; p < node.primitive_count; ++p)
        {
            const auto& primitive = m_primitives[offset + p];
            node.aabb_min         = pointwise_min(node.aabb_min, primitive->aabb_min());
            node.aabb_max         = pointwise_max(node.aabb_min, primitive->aabb_max());
        }
    }

  protected:
    bool                                             m_dirty{ true };
    std::vector<BVHNode>                             m_nodes;
    std::vector<std::shared_ptr<GeometricPrimitive>> m_primitives;
};

}; // namespace radiant