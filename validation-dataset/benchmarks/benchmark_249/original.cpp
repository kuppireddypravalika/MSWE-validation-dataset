#include "common.h"

bool intersectRay(const Ray& r, const AABox& bounds, float maxDist) {
    float intervalMin = -1.f;
    float intervalMax = -1.f;
    Vec3 org = r.origin();
    Vec3 dir = r.direction();
    Vec3 invDir;
    for (int i = 0; i < 3; ++i) {
        invDir[i] = 1.f / dir[i];
        if (fuzzyNe(dir[i], 0.0f)) {
            float t1 = (bounds.lo[i] - org[i]) * invDir[i];
            float t2 = (bounds.hi[i] - org[i]) * invDir[i];
            if (t1 > t2)
                std::swap(t1, t2);
            if (t1 > intervalMin)
                intervalMin = t1;
            if (t2 < intervalMax || intervalMax < 0.f)
                intervalMax = t2;
            if (intervalMax <= 0 || intervalMin >= maxDist)
                return false;
        }
    }
    if (intervalMin > intervalMax)
        return false;
    return true;
}
