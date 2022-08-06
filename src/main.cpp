#include <iostream>
#include <vector>

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include <stb_image.h>
#include <stb_image_write.h>

#include <vec3.h>
#include <ray.h>


Vec3 ray_color(const Ray &aRay) {
    float normalizedDirectionY = aRay.direction().normalize().y();
    float t = 0.5f * (normalizedDirectionY + 1);
    return (1 - t) * Vec3(1.0, 1.0, 1.0) + t * Vec3(0.5, 0.7, 1.0);
}

int main() {
    // Configure image parameters

    const float aspectRatio = 16.0f / 9.0f;
    const int32_t imageWidth = 1024;
    const auto imageHeight = static_cast<int32_t>(imageWidth / aspectRatio);

    // Configure camera parameters

    const float viewportHeight = 2.0f;
    const float viewportWidth = viewportHeight * aspectRatio;
    const float focalLength = 1.0f;

    Vec3 origin(0.0f, 0.0f, 0.0f);
    Vec3 vertical(0.0f, viewportHeight, 0.0f);
    Vec3 horizontal(viewportWidth, 0.0f, 0.0f);
    // Check this 3D math
    Vec3 lowerLeft = origin - vertical / 2 - horizontal / 2 - Vec3(0.0f, 0.0f, focalLength);

    // Create a buffer to hold the image
    std::vector<unsigned char> buffer(imageHeight * imageWidth * 3);

    int32_t index = 0;
    for (int32_t j = imageHeight - 1; j >= 0; --j) {
        std::cerr << "Progress: " << static_cast<float>(imageHeight - j) / imageHeight * 100 << "%\n" << std::flush;
        for (int32_t i = 0; i < imageWidth; ++i) {
            // Map to image-space coordinates
            float u = static_cast<float>(i) / (imageWidth - 1);
            float v = static_cast<float>(j) / (imageHeight - 1);

            // Check this 3D Math
            Ray r(origin, lowerLeft + u * horizontal + v * vertical - origin);
            Vec3 color = ray_color(r);

            buffer[index++] = static_cast<unsigned char>(255.999 * color.r());
            buffer[index++] = static_cast<unsigned char>(255.999 * color.g());
            buffer[index++] = static_cast<unsigned char>(255.999 * color.b());
        }
    }
    std::cerr << "\nDone.\n";

    stbi_write_jpg("../image.jpg", imageWidth, imageHeight, 3, buffer.data(), imageWidth * 3);
    return 0;
}
