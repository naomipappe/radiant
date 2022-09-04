#ifndef RAYTRACING1WEEKEND_MAIN_H
#define RAYTRACING1WEEKEND_MAIN_H

#include <iostream>
#include <vector>

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include <stb_image.h>
#include <stb_image_write.h>

#include <vec3.h>
#include <ray.h>
#include <sphere.h>

constexpr float infinity = std::numeric_limits<float>::infinity();
constexpr float pi = 3.1415926535897932385;

inline float deg2rad(float degrees) {
    return degrees * pi / 180.0;
}

#endif //RAYTRACING1WEEKEND_MAIN_H
