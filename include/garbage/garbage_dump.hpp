#pragma once

#include <cassert>
#include <cinttypes>
#include <filesystem>

#include <common/color.hpp>
#include <common/int_types.hpp>
// I call this file "garbage dump" so I feel more inclined to figure out where it actually should be
namespace radiant::garbage
{

// Writes the data into ppm image
// Assumes that bytes is of length width * height * 3
// Also assumes that the data in the image is written row by row
void write_ppm(const u32* bytes, u32 width, u32 height, std::filesystem::path& destination);

void write_ppm(const rgb_color* pixels, u32 width, u32 height, std::filesystem::path& destination)
{
    std::ofstream destination_file(destination, std::ios_base::out | std::ios_base::trunc);
    assert(destination_file);

    destination_file << "P3\n" << width << " " << height << "\n255\n";

    for (u32 i = 0; i < width * height; ++i)
    {
        if (i % height == 0)
        {
            destination_file << '\n';
        }
        destination_file << pixels[i][0] << ' ' << pixels[i][1] << ' ' << pixels[i][2] << ' ';
    }
    destination_file.close();
}

} // namespace radiant::garbage