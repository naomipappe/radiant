#ifndef RAYTRACING1WEEKEND_HITTABLE_H
#define RAYTRACING1WEEKEND_HITTABLE_H

#include <ray.h>

struct HitRecord {
    Vec3 mHitPoint;
    Vec3 mNormalAtHitPoint;
    float t;
    bool mHitFrontFace;

    inline void setFaceNormal(const Ray &aRay, const Vec3 &outwardNormal) {
        // If the dot product between vectors is less than zero, then they point in the opposite directions
        // Hence, we hit the front face of the geometry
        mHitFrontFace = aRay.direction().dot(outwardNormal) < 0.0f;
        mNormalAtHitPoint = mHitFrontFace ? outwardNormal : -outwardNormal;
    }
};

class Hittable {
public:
    virtual bool hit(const Ray &aRay, float tMin, float tMax, HitRecord &aHistory) const = 0;
};


#endif //RAYTRACING1WEEKEND_HITTABLE_H
