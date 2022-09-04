#include <main.h>

#include <hittable_list.h>


// Here, we compute at which point in time the ray would hit the sphere
Vec3 ray_color(const Ray &aRay, const Hittable &world) {
    HitRecord record;
    bool hit = world.hit(aRay, 0.0, infinity, record);
    if (hit) {
        return 0.5 * (record.mNormalAtHitPoint + 1);
    }
    // Project the [-1,1] y segment into [0,1] segment in order to use lerp for color blending
    float t = 0.5f * (aRay.direction().normalize().y() + 1);
    return (1 - t) * Vec3(1.0, 1.0, 1.0) + t * Vec3(0.5, 0.7, 1.0);
}

int main() {
    // Configure image parameters

    const float aspectRatio = 16.0f / 9.0f;
    const int32_t imageWidth = 1920;
    const auto imageHeight = static_cast<int32_t>(imageWidth / aspectRatio);

    HittableList world;
    world.add(std::make_shared<Sphere>(Vec3(0, 0, -1), 0.5));
    world.add(std::make_shared<Sphere>(Vec3(0, -100.5, -1), 100));

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
            Vec3 color = ray_color(r, world);

            buffer[index++] = static_cast<unsigned char>(255.999 * color.r());
            buffer[index++] = static_cast<unsigned char>(255.999 * color.g());
            buffer[index++] = static_cast<unsigned char>(255.999 * color.b());
        }
    }
    std::cerr << "\nDone.\n";

    stbi_write_jpg("../image.jpg", imageWidth, imageHeight, 3, buffer.data(), imageWidth * 3);
    return 0;
}
