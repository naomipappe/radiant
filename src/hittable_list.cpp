#include <hittable_list.h>

HittableList::HittableList() {

}

void HittableList::clear() {
    mHittables.clear();
}

void HittableList::add(const std::shared_ptr<Hittable> &aHittable) {
    mHittables.push_back(aHittable);
}

bool HittableList::hit(const Ray &aRay, float tMin, float tMax, HitRecord &aHistory) const {
    HitRecord record;
    bool hit = false;
    float closest = tMax;

    for (const auto &object: mHittables) {
        if (object->hit(aRay, tMin, closest, record)) {
            hit = true;
            if (record.t < closest) {
                aHistory = record;
                closest = aHistory.t;
            }
        }

    }
    return hit;
}