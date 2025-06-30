#pragma once

#include <algorithm>
#include <span>
#ifndef RADIANT_BUFFER_CACHE_HPP
#define RADIANT_BUFFER_CACHE_HPP

// The idea and implementation is taken almost directly from pbrt-v4
// (https://github.com/mmp/pbrt-v4/blob/master/src/pbrt/util/buffercache.h) I do not know if I really need this but it is
// easier to have it for now and think later

#include "core/types.hpp"
#include <unordered_set>

namespace radiant
{
template <typename T>
class BufferStorage
{
  public:
    static BufferStorage<T>& instance()
    {
        static BufferStorage<T> instance_;
        return instance_;
    }
    BufferStorage(const BufferStorage&)            = delete;
    BufferStorage& operator=(const BufferStorage&) = delete;

    const T* store(std::span<const T> data)
    {
        Buffer buffer(data);
        auto   result = storage_.find(buffer);
        if (result != storage_.end())
        {
            return result->m_data;
        }
        T* internal_copy = new T[data.size()]();
        std::copy(data.begin(), data.end(), internal_copy);
        storage_.emplace(std::span{ internal_copy, data.size() });
        return internal_copy;
    }

  private:
    struct Buffer
    {
        const T* m_data{ nullptr };
        u64      m_size{ 0 };
        u64      m_hash{};

        Buffer() = default;
        Buffer(std::span<const T> data) : m_data(data.data()), m_size(data.size()) { m_hash = hash(*this); }

        bool operator==(const Buffer& b) const
        {
            return m_size == b.m_size && m_hash == b.m_hash && std::memcmp(m_data, b.m_data, m_size * sizeof(T)) == 0;
        }
    };

    struct BufferHasher
    {
        u64 operator()(const Buffer& b) const { return b.m_hash; }
    };

    // https://github.com/explosion/murmurhash/blob/master/murmurhash/MurmurHash2.cpp
    static u64 hash(const Buffer& buffer, u64 seed = 0)
    {
        const u8* data = reinterpret_cast<const u8*>(buffer.m_data);
        const u64 m    = 0xc6a4a7935bd1e995ull;
        const int r    = 47;

        u64 h = seed ^ (buffer.m_size * m);

        const unsigned char* end = data + 8 * (buffer.m_size / 8);

        while (data != end)
        {
            u64 k;
            std::memcpy(&k, data, sizeof(u64));
            data += 8;

            k *= m;
            k ^= k >> r;
            k *= m;

            h ^= k;
            h *= m;
        }

        switch (buffer.m_size & 7)
        {
            case 7:
                h ^= u64(data[6]) << 48;
            case 6:
                h ^= u64(data[5]) << 40;
            case 5:
                h ^= u64(data[4]) << 32;
            case 4:
                h ^= u64(data[3]) << 24;
            case 3:
                h ^= u64(data[2]) << 16;
            case 2:
                h ^= u64(data[1]) << 8;
            case 1:
                h ^= u64(data[0]);
                h *= m;
        };

        h ^= h >> r;
        h *= m;
        h ^= h >> r;

        return h;
    }

  private:
    BufferStorage()  = default;
    ~BufferStorage() = default;

  private:
    std::unordered_set<Buffer, BufferHasher> storage_;
};
} // namespace radiant

#endif