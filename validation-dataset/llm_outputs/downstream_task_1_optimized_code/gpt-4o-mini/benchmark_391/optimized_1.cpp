#include <vector>
#include <cstddef>
#include <cstdint>
#include <unordered_map>

struct QMetaObject { int id; };

struct QObject {
    const QMetaObject* meta;
    void* attached = nullptr;
};

using QQmlAttachedPropertiesFunc = QObject*(*)(QObject*, bool);

struct RegistryEntry {
    const QMetaObject* meta;
    QQmlAttachedPropertiesFunc func;
};

static std::vector<RegistryEntry> registry;
static std::unordered_map<const QMetaObject*, QQmlAttachedPropertiesFunc> lookup_map;

// Dummy attached object function used for most registry entries
static QObject* dummy_attached(QObject* obj, bool create) {
    return obj; // no-op
}

// Initialize registry with many dummy entries to make lookup efficient
static void init_registry() {
    if (!registry.empty()) return;
    const int N = 2000;
    registry.reserve(N + 1);
    for (int i = 0; i < N; ++i) {
        static QMetaObject mo{ i };
        registry.push_back({ &mo, dummy_attached });
        lookup_map[&mo] = dummy_attached;
    }
    registry.push_back({ &TypeA::staticMetaObject, dummy_attached });
    lookup_map[&TypeA::staticMetaObject] = dummy_attached;
}

// Lookup function that uses the unordered_map
QQmlAttachedPropertiesFunc qmlAttachedPropertiesFunction(QObject*, const QMetaObject* mo) {
    auto it = lookup_map.find(mo);
    return it != lookup_map.end() ? it->second : nullptr;
}

// Call the attached property function
QObject* qmlAttachedPropertiesObject(QObject* obj, QQmlAttachedPropertiesFunc func, bool create = true) {
    return func ? func(obj, create) : nullptr;
}

struct TypeA { static QMetaObject staticMetaObject; };
QMetaObject TypeA::staticMetaObject{10001};

// The optimized templated helper under test
template<typename T>
QObject* qmlAttachedPropertiesObject(const QObject* obj, bool create = true)
{
    QObject* mutableObj = const_cast<QObject*>(obj);
    return qmlAttachedPropertiesObject(
        mutableObj,
        qmlAttachedPropertiesFunction(mutableObj, &T::staticMetaObject),
        create);
}

size_t run(size_t loops) {
    init_registry();
    QObject obj{ &TypeA::staticMetaObject };  
    size_t sum = 0;
    for (size_t i = 0; i < loops; ++i) {
        // Count non-null results to keep output deterministic
        sum += qmlAttachedPropertiesObject<TypeA>(&obj) != nullptr;
    }
    return sum;
}

// Explicit template instantiation as needed
template QObject* qmlAttachedPropertiesObject<TypeA>(const QObject*, bool);
