#include <cstdio>

#include <garbage_dump.hpp>

#include <random>

int main()
{
    printf("%s", "Hello World!\n");

    int32_t image[256 * 256 * 3];
    for (uint32_t i = 0; i < 256 * 256 * 3; ++i)
    {
        image[i] = float(rand()) / RAND_MAX * 255;
    }
    std::filesystem::path out("../../test.ppm");
    radiant::garbage::write_ppm(image, 256, 256, out);

    return 0;
}