#pragma once

#include <cassert>
#include <cinttypes>
#include <filesystem>

#include <core/color.hpp>
#include <core/int_types.hpp>
// I call this file "garbage dump" so I feel more inclined to figure out where it actually should be
namespace radiant::garbage
{

// Writes the data into ppm image
// Assumes that bytes is of length width * height * 3
// Also assumes that the data in the image is written row by row
void write_ppm(const u32* bytes, u32 width, u32 height, std::filesystem::path& destination);
void write_ppm(const rgb_color* bytes, u32 width, u32 height, std::filesystem::path& destination);

void write_png(const rgb_color* pixels, u32 width, u32 height, std::filesystem::path& destination);
} // namespace radiant::garbage