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

static std::unordered_map<const QMetaObject*, QQmlAttachedPropertiesFunc> registry;

// Dummy attached object function used for most registry entries
static QObject* dummy_attached(QObject* obj, bool create) {
    return obj; // no-op
}

// Lookup function that uses a hashmap for fast retrieval
QQmlAttachedPropertiesFunc qmlAttachedPropertiesFunction(QObject*, const QMetaObject* mo) {
    auto it = registry.find(mo);
    return (it != registry.end()) ? it->second : nullptr;
}

// Call the attached property function
QObject* qmlAttachedPropertiesObject(QObject* obj, QQmlAttachedPropertiesFunc func, bool create = true) {
    return func ? func(obj, create) : nullptr;
}

struct TypeA { static QMetaObject staticMetaObject; };
QMetaObject TypeA::staticMetaObject{10001};

// The templated helper under test (optimized version)
template<typename T>
QObject* qmlAttachedPropertiesObject(const QObject* obj, bool create = true) {
    QObject* mutableObj = const_cast<QObject*>(obj);
    auto func = qmlAttachedPropertiesFunction(mutableObj, &T::staticMetaObject);
    return qmlAttachedPropertiesObject(mutableObj, func, create);
}

// Initialize registry with many dummy entries to make lookup efficient
static void init_registry() {
    if (!registry.empty()) return;
    const int N = 2000;
    for (int i = 0; i < N; ++i) {
        static QMetaObject mo{ i };
        registry[&mo] = dummy_attached;
    }
    registry[&TypeA::staticMetaObject] = dummy_attached;
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

// Explicit template instantiation statements
template QObject* qmlAttachedPropertiesObject<TypeA>(const QObject*, bool);
