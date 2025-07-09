#include "intersector.hpp"

// optimized version delays the expensive t calculation until needed
static inline bool intersect_triangle(const Triangle& tri, const Ray& ray, float& t) {
    Vec3 e1 = tri.v0 - tri.v1;
    Vec3 e2 = tri.v2 - tri.v0;
    Vec3 C  = {tri.v0.x - ray.org.x, tri.v0.y - ray.org.y, tri.v0.z - ray.org.z};
    Vec3 Ng = tri.Ng;
    float den = dot(ray.dir, Ng);
    float rcp_den = 1.0f / den;
    Vec3 R = cross(ray.dir, C);
    float u = dot(R, e2) * rcp_den;
    float v = dot(R, e1) * rcp_den;
    bool valid = (u >= 0.0f) && (v >= 0.0f) && (u + v <= 1.0f);
    if (!valid) return false; // skip heavy computation when outside triangle
    t = compute_intersection_distance(C, Ng, rcp_den);
    if (t < ray.tnear || t > ray.tfar) return false;
    return true;
}

float run(size_t loops, const std::vector<Triangle>& tris, const Ray& ray) {
    float sum = 0.0f;
    for (size_t i = 0; i < loops; ++i) {
        for (const Triangle& tri : tris) {
            float t;
            if (intersect_triangle(tri, ray, t))
                sum += t;
        }
    }
    return sum;
}
