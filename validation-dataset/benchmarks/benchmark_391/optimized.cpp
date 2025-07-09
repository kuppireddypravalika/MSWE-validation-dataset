#include <vector>
#include <cstddef>
#include <cstdint>

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

static QObject* dummy_attached(QObject* obj, bool create) {
    return obj;
}

QQmlAttachedPropertiesFunc qmlAttachedPropertiesFunction(QObject*, const QMetaObject* mo) {
    for (const auto& entry : registry) {
        if (entry.meta == mo)
            return entry.func;
    }
    return nullptr;
}

QObject* qmlAttachedPropertiesObject(QObject* obj, QQmlAttachedPropertiesFunc func, bool create = true) {
    return func ? func(obj, create) : nullptr;
}

struct TypeA { static QMetaObject staticMetaObject; };
QMetaObject TypeA::staticMetaObject{10001};

// Optimized templated helper with cached function pointer
template<typename T>
QObject* qmlAttachedPropertiesObject(const QObject* obj, bool create = true)
{
    static const auto func = qmlAttachedPropertiesFunction(nullptr, &T::staticMetaObject);
    return qmlAttachedPropertiesObject(const_cast<QObject*>(obj), func, create);
}

static void init_registry() {
    if (!registry.empty()) return;
    const int N = 2000;
    registry.reserve(N + 1);
    for (int i = 0; i < N; ++i) {
        static QMetaObject mo{ i };
        registry.push_back({ &mo, dummy_attached });
    }
    registry.push_back({ &TypeA::staticMetaObject, dummy_attached });
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

