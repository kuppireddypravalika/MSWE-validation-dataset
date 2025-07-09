#include <chrono>
#include <iostream>
#include <string>
#include <vector>
#include <memory>

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

bool intersect_triangle_triangle(TutorialScene*, unsigned, unsigned, TutorialScene*, unsigned, unsigned);

TutorialScene createScene(){
    TutorialScene scene;
    auto mesh = std::make_shared<SceneGraph::TriangleMeshNode>();
    mesh->positions = { {0,0,0}, {1,0,0}, {0,1,0}, {0,0,1} };
    mesh->triangles = { {0,1,2}, {0,2,3} };
    Geometry g; g.ref = mesh; g.ptr = mesh.get();
    scene.geometries.push_back(g);
    return scene;
}

int main(int argc, char* argv[]){
    std::string mode = "perf";
    long long repeat = 300000000; // default workload

    for(int i=1;i<argc;++i){
        std::string arg = argv[i];
        if(arg=="--mode=correct") mode="correct";
        else if(arg=="--mode=perf") mode="perf";
        else if(arg.rfind("--repeat=",0)==0) repeat=std::stoll(arg.substr(9));
    }

    TutorialScene scene0 = createScene();
    TutorialScene scene1 = createScene();

    if(mode=="correct"){
        bool res = intersect_triangle_triangle(&scene0,0,0,&scene1,0,1);
        std::cout << res << std::endl;
    } else {
        using clock = std::chrono::high_resolution_clock;
        auto t1 = clock::now();
        for(long long i=0;i<repeat;++i)
            intersect_triangle_triangle(&scene0,0,0,&scene1,0,1);
        auto t2 = clock::now();

        std::cout << "Time: "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(t2-t1).count()
                  << " ms\n";
    }
}
