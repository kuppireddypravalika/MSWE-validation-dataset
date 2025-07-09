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

// Original implementation using a copy of the child vector
template<typename T>
T* findDirectChild(Object* obj, const std::string& aName = std::string())
{
    auto copy = obj->children(); // copies the vector like Qt foreach
    for (Object* child : copy) {
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
