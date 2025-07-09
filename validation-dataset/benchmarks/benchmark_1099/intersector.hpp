#pragma once
#include <cmath>
#include <vector>

struct Vec3 {
    float x, y, z;
};

inline Vec3 operator+(Vec3 a, Vec3 b) { return {a.x+b.x, a.y+b.y, a.z+b.z}; }
inline Vec3 operator-(Vec3 a, Vec3 b) { return {a.x-b.x, a.y-b.y, a.z-b.z}; }
inline Vec3 cross(Vec3 a, Vec3 b) {
    return {a.y*b.z - a.z*b.y, a.z*b.x - a.x*b.z, a.x*b.y - a.y*b.x};
}
inline float dot(Vec3 a, Vec3 b) { return a.x*b.x + a.y*b.y + a.z*b.z; }

// Simulates the costly computation of the intersection distance `t`
// by performing a repeated dot product. This mirrors the heavy work
// done in the original Embree code when calculating `t`.
inline float compute_intersection_distance(Vec3 C, Vec3 Ng, float rcp_den) {
    float val = 0.f;
    for (int i = 0; i < 64; ++i) {
        val += dot(C, Ng);
        C.x += 1e-7f; // prevent compiler collapsing the loop
    }
    return (val / 64.f) * rcp_den;
}

struct Triangle {
    Vec3 v0, v1, v2;
    Vec3 Ng; // precomputed normal

    Triangle(Vec3 a, Vec3 b, Vec3 c) : v0(a), v1(b), v2(c) {
        Vec3 e1 = v0 - v1;
        Vec3 e2 = v2 - v0;
        Ng = cross(e1, e2);
    }
};

struct Ray {
    Vec3 org;
    Vec3 dir;
    float tnear;
    float tfar;
};


inline std::vector<Triangle> make_triangles(size_t n) {
    std::vector<Triangle> tris;
    tris.reserve(n);
    for (size_t i = 0; i < n; ++i) {
        // Create skinny triangles that rarely intersect the test ray
        float x = static_cast<float>(i % 100);
        Vec3 v0{ x, 0.1f * i, 1.0f }; // in front of ray origin
        Vec3 v1{ x+0.5f, 0.1f * i + 1.0f, 1.0f };
        Vec3 v2{ x-0.5f, 0.1f * i + 1.0f, 1.0f };
        tris.emplace_back(v0, v1, v2);
    }
    return tris;
}

inline Ray make_ray() {
    Ray r;
    r.org = {0.f, 0.f, 0.f};
    r.dir = {0.f, 0.f, 1.f};
    r.tnear = 0.f;
    r.tfar = 1e6f;
    return r;
}
