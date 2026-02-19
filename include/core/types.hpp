#pragma once

#include <cstdint>
#include <concepts>

namespace radiant
{
using u8  = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;

using i8  = int8_t;
using i16 = int16_t;
using i32 = int32_t;
using i64 = int64_t;

using f32 = float;
using f64 = double;

using Scalar = f64;

template <typename V>
concept ScalarT = (std::is_integral_v<V> && !std::is_same_v<V, bool>) || std::is_floating_point_v<V>;
} // namespace radiant
