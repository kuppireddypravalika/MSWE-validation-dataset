#ifndef BENCHMARK_249_COMMON_H
#define BENCHMARK_249_COMMON_H
#include <cmath>
#include <cstdint>

struct Vec3 {
    float v[3];
    float& operator[](int i) { return v[i]; }
    const float& operator[](int i) const { return v[i]; }
};

struct Ray {
    Vec3 o;
    Vec3 d;
    Vec3 invd;
    Ray() = default;
    Ray(const Vec3& org, const Vec3& dir) : o(org), d(dir) {
        for (int i = 0; i < 3; ++i)
            invd[i] = 1.f / d[i];
    }
    const Vec3& origin() const { return o; }
    const Vec3& direction() const { return d; }
    const Vec3& invDirection() const { return invd; }
};

struct AABox {
    Vec3 lo;
    Vec3 hi;
};

inline bool fuzzyNe(float a, float b) {
    return std::fabs(a - b) > 1e-6f;
}

#endif
