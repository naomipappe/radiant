#ifndef RAYTRACING1WEEKEND_HITTABLE_H
#define RAYTRACING1WEEKEND_HITTABLE_H

#include <ray.h>

struct HitRecord {
    Vec3 mHitPoint;
    Vec3 mNormalAtHitPoint;
    float t;
};

class Hittable {
public:
    virtual bool hit(const Ray &aRay, float tMin, float tMax, HitRecord &aHistory) const = 0;
};


#endif //RAYTRACING1WEEKEND_HITTABLE_H
