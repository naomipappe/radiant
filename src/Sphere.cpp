#include <sphere.h>

Sphere::Sphere() : Sphere(Vec3(0.f, 0.f, 0.f), 1.0f) {

}

Sphere::Sphere(const Vec3 &aCenter, float aRadius)
        : mCenter(aCenter), mRadius(aRadius) {

}

bool Sphere::hit(const Ray &aRay, float tMin, float tMax, HitRecord &aHistory) const {
    Vec3 oc = aRay.origin() - mCenter;
    float a = aRay.direction().len_squared();
    float hb = oc.dot(aRay.direction());
    float c = oc.len_squared() - mRadius * mRadius;
    float discriminant = hb * hb - a * c;
    if (discriminant < 0) {
        return false;
    } else {
        discriminant = std::sqrt(discriminant);
        float root = (-hb - discriminant) / a;
        if (root < tMin || root > tMax) {
            // Try the other one
            root = (-hb + discriminant) / a;
            if (root < tMin || root > tMax) {
                return false;
            }
        }
        aHistory.t = root;
        aHistory.mHitPoint = aRay.at(root);
        aHistory.setFaceNormal(aRay, (aHistory.mHitPoint - mCenter).normalize());
        return true;
    }
}
