#include <memory>
#include <vector>
#include <cmath>

struct Vec3 { float x, y, z; };
inline Vec3 operator-(const Vec3& a, const Vec3& b){ return {a.x-b.x,a.y-b.y,a.z-b.z}; }
inline Vec3 cross(const Vec3& a, const Vec3& b){ return {a.y*b.z-a.z*b.y, a.z*b.x-a.x*b.z, a.x*b.y-a.y*b.x}; }
inline float dot(const Vec3& a, const Vec3& b){ return a.x*b.x + a.y*b.y + a.z*b.z; }

namespace SceneGraph {
struct Node { virtual ~Node() = default; };
struct TriangleMeshNode : Node {
    struct Triangle { unsigned v0,v1,v2; };
    std::vector<Vec3> positions;
    std::vector<Triangle> triangles;
};
}

struct Geometry {
    std::shared_ptr<SceneGraph::Node> ref;
    SceneGraph::Node* ptr;
    template<typename T>
    std::shared_ptr<T> dynamicCast() const { return std::dynamic_pointer_cast<T>(ref); }
};

struct TutorialScene { std::vector<Geometry> geometries; };

static bool tri_intersect(const Vec3& a0,const Vec3& a1,const Vec3& a2,
                          const Vec3& b0,const Vec3& b1,const Vec3& b2){
    Vec3 n1 = cross(a1-a0, a2-a0);
    Vec3 n2 = cross(b1-b0, b2-b0);
    return dot(n1,n2) >= 0.f;
}

bool intersect_triangle_triangle(TutorialScene* scene0, unsigned geomID0, unsigned primID0,
                                 TutorialScene* scene1, unsigned geomID1, unsigned primID1)
{
    auto mesh0 = scene0->geometries[geomID0].dynamicCast<SceneGraph::TriangleMeshNode>();
    auto mesh1 = scene1->geometries[geomID1].dynamicCast<SceneGraph::TriangleMeshNode>();
    const auto& tri0 = mesh0->triangles[primID0];
    const auto& tri1 = mesh1->triangles[primID1];

    const Vec3& a0 = mesh0->positions[tri0.v0];
    const Vec3& a1 = mesh0->positions[tri0.v1];
    const Vec3& a2 = mesh0->positions[tri0.v2];
    const Vec3& b0 = mesh1->positions[tri1.v0];
    const Vec3& b1 = mesh1->positions[tri1.v1];
    const Vec3& b2 = mesh1->positions[tri1.v2];

    return tri_intersect(a0,a1,a2,b0,b1,b2);
}
