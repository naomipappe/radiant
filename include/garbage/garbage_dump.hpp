#pragma once

#include "core/vec.hpp"
#include <core/types.hpp>

#include <filesystem>

// I call this file "garbage dump" so I feel more inclined to figure out where it actually should be
namespace radiant::garbage
{

Scalar linear_to_gamma(Scalar linear_component);

// Writes the data into ppm image
// Assumes that bytes is of length width * height * 3
// Also assumes that the data in the image is written row by row
void write_ppm(const vec3* pixels, u32 width, u32 height, std::filesystem::path& destination);
void write_png(const vec3* pixels, u32 width, u32 height, std::filesystem::path& destination);
void write_jpg(const vec3* pixels, u32 width, u32 height, std::filesystem::path& destination);
} // namespace radiant::garbage