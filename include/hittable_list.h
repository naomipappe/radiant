#ifndef RAYTRACING1WEEKEND_HITTABLE_LIST_H
#define RAYTRACING1WEEKEND_HITTABLE_LIST_H

#include <vector>
#include <memory>

#include <hittable.h>

class HittableList : public Hittable {
public:
    HittableList();

    void clear();

    void add(const std::shared_ptr<Hittable>& aHittable);

    virtual bool hit(const Ray &aRay, float tMin, float tMax, HitRecord &aHistory) const override;

private:
    std::vector<std::shared_ptr<Hittable>> mHittables;
};

#endif //RAYTRACING1WEEKEND_HITTABLE_LIST_H


