#ifndef RAYTRACING1WEEKEND_VEC3_H
#define RAYTRACING1WEEKEND_VEC3_H

#include <cmath>

class Vec3;

using std::sqrt;

class Vec3 {
public:
    Vec3() : mData{0.0f, 0.0f, 0.0f} {}

    Vec3(float x, float y, float z) : mData{x, y, z} {}

    Vec3(const Vec3 &other) {
        mData[0] = other.mData[0];
        mData[1] = other.mData[1];
        mData[2] = other.mData[2];
    }

    [[nodiscard]] inline float x() const { return mData[0]; }

    [[nodiscard]] inline float y() const { return mData[1]; }

    [[nodiscard]] inline float z() const { return mData[2]; }

    [[nodiscard]] inline float r() const { return mData[0]; }

    [[nodiscard]] inline float g() const { return mData[1]; }

    [[nodiscard]] inline float b() const { return mData[2]; }

    float operator[](const size_t index) const {
        return mData[index];
    }

    float &operator[](const size_t index) {
        return mData[index];
    }

    Vec3 operator-() const { return {-mData[0], -mData[1], -mData[2]}; }

    Vec3 &operator+=(const Vec3 &other) {
        mData[0] += other.mData[0];
        mData[1] += other.mData[1];
        mData[2] += other.mData[2];
        return *this;
    }

    Vec3 &operator+=(const float t) {
        mData[0] += t;
        mData[1] += t;
        mData[2] += t;
        return *this;
    }

    inline friend Vec3 operator+(const Vec3 &left, const Vec3 &right) {
        return Vec3{
                left.mData[0] + right.mData[0],
                left.mData[1] + right.mData[1],
                left.mData[2] + right.mData[2]
        };
    }

    inline friend Vec3 operator+(const Vec3 &left, const float t) {
        return Vec3{
                left.mData[0] + t,
                left.mData[1] + t,
                left.mData[2] + t
        };
    }

    inline friend Vec3 operator+(const float t, const Vec3 &vec) {
        return Vec3{
                vec.mData[0] + t,
                vec.mData[1] + t,
                vec.mData[2] + t
        };
    }

    Vec3 &operator-=(const Vec3 &other) {
        mData[0] -= other.mData[0];
        mData[1] -= other.mData[1];
        mData[2] -= other.mData[2];
        return *this;
    }

    Vec3 &operator-=(const float t) {
        mData[0] -= t;
        mData[1] -= t;
        mData[2] -= t;
        return *this;
    }

    inline friend Vec3 operator-(const Vec3 &left, const Vec3 &right) {
        return Vec3{
                left.mData[0] - right.mData[0],
                left.mData[1] - right.mData[1],
                left.mData[2] - right.mData[2]
        };
    }

    inline friend Vec3 operator-(const Vec3 &left, const float t) {
        return Vec3{
                left.mData[0] - t,
                left.mData[1] - t,
                left.mData[2] - t
        };
    }

    inline friend Vec3 operator-(const float t, const Vec3 &vec) {
        return Vec3{
                vec.mData[0] - t,
                vec.mData[1] - t,
                vec.mData[2] - t
        };
    }

    Vec3 &operator*=(const Vec3 &other) {
        mData[0] *= other.mData[0];
        mData[1] *= other.mData[1];
        mData[2] *= other.mData[2];
        return *this;
    }

    Vec3 &operator*=(const float t) {
        mData[0] *= t;
        mData[1] *= t;
        mData[2] *= t;
        return *this;
    }

    inline friend Vec3 operator*(const Vec3 &left, const Vec3 &right) {
        return Vec3{
                left.mData[0] * right.mData[0],
                left.mData[1] * right.mData[1],
                left.mData[2] * right.mData[2]
        };
    }

    inline friend Vec3 operator*(const Vec3 &vec, const float t) {
        return Vec3{
                vec.mData[0] * t,
                vec.mData[1] * t,
                vec.mData[2] * t
        };
    }

    inline friend Vec3 operator*(const float t, const Vec3 &vec) {
        return Vec3{
                vec.mData[0] * t,
                vec.mData[1] * t,
                vec.mData[2] * t
        };
    }

    Vec3 &operator/=(const Vec3 &other) {
        mData[0] /= other.mData[0];
        mData[1] /= other.mData[1];
        mData[2] /= other.mData[2];
        return *this;
    }

    Vec3 &operator/=(const float t) {
        mData[0] /= t;
        mData[1] /= t;
        mData[2] /= t;
        return *this;
    }

    inline friend Vec3 operator/(const Vec3 &left, const Vec3 &right) {
        return Vec3{
                left.mData[0] / right.mData[0],
                left.mData[1] / right.mData[1],
                left.mData[2] / right.mData[2]
        };
    }

    inline friend Vec3 operator/(const Vec3 &vec, const float t) {
        return Vec3{
                vec.mData[0] / t,
                vec.mData[1] / t,
                vec.mData[2] / t
        };
    }

    inline friend Vec3 operator/(const float t, const Vec3 &vec) {
        return Vec3{
                vec.mData[0] / t,
                vec.mData[1] / t,
                vec.mData[2] / t
        };
    }

    [[nodiscard]] float dot(const Vec3 &other) const {
        return this->mData[0] * other.mData[0] + this->mData[1] * other.mData[1] + this->mData[2] * other.mData[2];
    }

    [[nodiscard]] float len() const {
        return sqrt(dot(*this));
    }

    [[nodiscard]] float len_squared() const {
        return dot(*this);
    }

    inline friend Vec3 cross(const Vec3 &u, const Vec3 &v) {
        return {u.mData[1] * v.mData[2] - u.mData[2] * v.mData[1],
                    u.mData[2] * v.mData[0] - u.mData[0] * v.mData[2],
                    u.mData[0] * v.mData[1] - u.mData[1] * v.mData[0]};
    }

    [[nodiscard]] inline Vec3 normalize() const {
        return *this / this->len();
    }

private:
    float mData[3]{};
};


#endif //RAYTRACING1WEEKEND_VEC3_H
