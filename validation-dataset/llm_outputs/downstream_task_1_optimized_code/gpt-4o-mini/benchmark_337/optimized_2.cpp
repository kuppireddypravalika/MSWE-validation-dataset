#include <string>
#include <vector>

struct Object {
    std::string name;
    std::vector<Object*> kids;
    virtual ~Object() = default;
    const std::vector<Object*>& children() const { return kids; }
    const std::string& objectName() const { return name; }
};

struct Widget : Object {
    using Object::Object;
};

// Optimized implementation without copying the child vector
template<typename T>
T* findDirectChild(Object* obj, const std::string& aName = std::string())
{
    // Only reference the vector of children directly to avoid an unnecessary copy.
    for (Object* child : obj->children()) {
        T* qchild = dynamic_cast<T*>(child);
        if (qchild != nullptr) {
            if (aName.empty() || qchild->objectName() == aName)
                return qchild;
        }
    }
    return nullptr;
}

// Explicit instantiation for the benchmark
template Widget* findDirectChild<Widget>(Object*, const std::string&);