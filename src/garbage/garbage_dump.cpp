#include <algorithm>
#include <cmath>
#include <fstream>

#include <stb_image_write.h>

#include <garbage/garbage_dump.hpp>

namespace radiant::garbage
{

f32 linear_to_gamma(f32 linear_component)
{
    if (linear_component > 0)
    {
        return std::sqrt(linear_component);
    }
    return 0.0f;
}

void write_ppm(const u32* bytes, u32 width, u32 height, std::filesystem::path& destination)
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
        destination_file << bytes[3 * i] << ' ' << bytes[3 * i + 1] << ' ' << bytes[3 * i + 2] << ' ';
    }
    destination_file.close();
}

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
        destination_file << pixels[i];
    }
    destination_file.close();
}

void write_png(const rgb_color* pixels, u32 width, u32 height, std::filesystem::path& destination)
{
    // TODO: This is messy because I use std::filesystem::path and it uses wchar
    const std::string temporary = destination.string();

    // TODO: This is messy because I do not store the image I am writing into as a continuous memory, change that
    // TODO: For now, this means converting the image into appropriate format
    std::vector<u8> converted_data(width * height * 3, 1);
    for (int i = 0; i < width * height; i++)
    {
        converted_data[i * 3 + 0] = std::clamp(linear_to_gamma(pixels[i].r()), 0.0f, 1.0f) * 255; // R
        converted_data[i * 3 + 1] = std::clamp(linear_to_gamma(pixels[i].g()), 0.0f, 1.0f) * 255; // G
        converted_data[i * 3 + 2] = std::clamp(linear_to_gamma(pixels[i].b()), 0.0f, 1.0f) * 255; // B
    }

    stbi_write_png(temporary.c_str(), width, height, 3, converted_data.data(), width * 3);
}

void write_jpg(const rgb_color* pixels, u32 width, u32 height, std::filesystem::path& destination)
{
    // TODO: This is messy because I use std::filesystem::path and it uses wchar
    const std::string temporary = destination.string();

    // TODO: This is messy because I do not store the image I am writing into as a continuous memory, change that
    // TODO: For now, this means converting the image into appropriate format
    std::vector<u8> converted_data(width * height * 3, 1);
    for (int i = 0; i < width * height; i++)
    {
        converted_data[i * 3 + 0] = std::clamp(linear_to_gamma(pixels[i].r()), 0.0f, 1.0f) * 255; // R
        converted_data[i * 3 + 1] = std::clamp(linear_to_gamma(pixels[i].g()), 0.0f, 1.0f) * 255; // G
        converted_data[i * 3 + 2] = std::clamp(linear_to_gamma(pixels[i].b()), 0.0f, 1.0f) * 255; // B
    }

    stbi_write_jpg(temporary.c_str(), width, height, 3, converted_data.data(), width * 3);
}

} // namespace radiant::garbage
