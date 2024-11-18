#pragma once

#include <cassert>
#include <cinttypes>
#include <filesystem>

// I call this file "garbage dump" so I feel more inclined to figure out where it actually should be
namespace radiant::garbage
{

// Writes the data into ppm image
// Assumes that bytes is of length width * height * 3
// Also assumes that the data in the image is written row by row
void write_ppm(int32_t* bytes, uint32_t width, uint32_t height, std::filesystem::path& destination);
}