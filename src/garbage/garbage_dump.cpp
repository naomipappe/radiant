#include <fstream>
#include <garbage/garbage_dump.hpp>

namespace radiant::garbage
{
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
} // namespace radiant::garbage
