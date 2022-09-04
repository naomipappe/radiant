#ifndef RAYTRACING1WEEKEND_SPHERE_H
#define RAYTRACING1WEEKEND_SPHERE_H

#include <hittable.h>

class Sphere : public Hittable {
public:
    Sphere();

    Sphere(const Vec3 &aCenter, float aRadius);

    virtual bool hit(const Ray &aRay, float tMin, float tMax, HitRecord &aHistory) const override;

private:
    Vec3 mCenter;
    float mRadius;
};

#endif //RAYTRACING1WEEKEND_SPHERE_H
