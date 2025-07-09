#include "common.h"

bool intersectRay(const Ray& r, const AABox& bounds, float maxDist) {
    float intervalMin = -1.f;
    float intervalMax = maxDist;
    const Vec3& org = r.origin();
    const Vec3& dir = r.direction();
    const Vec3& invDir = r.invDirection();

    for (int i = 0; i < 3; ++i) {
        if (fuzzyNe(dir[i], 0.0f)) {
            float t1 = (bounds.lo[i] - org[i]) * invDir[i];
            float t2 = (bounds.hi[i] - org[i]) * invDir[i];
            if (t1 > t2) std::swap(t1, t2);

            intervalMin = std::max(intervalMin, t1);
            intervalMax = std::min(intervalMax, t2);

            if (intervalMax <= intervalMin)
                return false;
        }
    }
    return intervalMin < maxDist;
}
