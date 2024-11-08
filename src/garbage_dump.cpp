#include <garbage_dump.hpp>

#include <fstream>

namespace radiant::garbage
{
void write_ppm(int32_t* bytes, uint32_t width, uint32_t height, std::filesystem::path& destination)
{
    std::ofstream destination_file(destination, std::ios_base::out | std::ios_base::trunc);
    assert(destination_file);

    destination_file << "P3\n" << width << " " << height << "\n255\n";

    for (uint32_t i = 0; i < width * height; ++i)
    {
        if (i % height == 0)
        {
            destination_file << '\n';
        }
        destination_file << bytes[3 * i] << ' ' << bytes[3 * i + 1] << ' ' << bytes[3 * i + 2] << ' ';
    }
    destination_file.close();
}
} // namespace radiant::garbage
