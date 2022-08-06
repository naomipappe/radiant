#ifndef RAYTRACING1WEEKEND_RAY_H
#define RAYTRACING1WEEKEND_RAY_H

#include <vec3.h>

class Ray {
public:
    Ray() = default;

    Ray(const Vec3 &aOrigin, const Vec3 &aDirection) : mOrigin(aOrigin), mDirection(aDirection) {}

    Vec3 origin() const { return mOrigin; }

    Vec3 direction() const { return mDirection; }

    Vec3 at(float t) { return mOrigin + t * mDirection; }

private:
    Vec3 mOrigin;
    Vec3 mDirection;
};

#endif //RAYTRACING1WEEKEND_RAY_H
