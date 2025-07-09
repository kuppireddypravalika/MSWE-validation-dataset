#include <cctype>
#include <string>
#include <unordered_map>
#include <vector>
#include <set>

struct ProtoElement {
    std::string typeName;
    bool isBasic;
};

struct ProtoClass {
    std::vector<ProtoElement> data;
    const std::vector<ProtoElement>& GetData() const { return data; }
};

class ClassTable {
public:
    static std::unordered_map<std::string, ProtoClass*> protoMap;

    static std::string Normalize(const std::string& name) {
        // Expensive normalization simulation
        std::string norm;
        norm.reserve(name.size());
        for (char c : name) {
            if (c != ' ') norm.push_back(std::tolower(static_cast<unsigned char>(c)));
        }
        return norm;
    }

    static void Register(const std::string& name, ProtoClass* proto) {
        protoMap[Normalize(name)] = proto;
    }

    static ProtoClass* GetProto(const char* name) {
        std::string norm(name);
        auto it = protoMap.find(norm);
        return it == protoMap.end() ? nullptr : it->second;
    }

    static ProtoClass* GetProtoNorm(const char* norm) {
        auto it = protoMap.find(norm);
        return it == protoMap.end() ? nullptr : it->second;
    }

    static bool GetDictNorm(const char* norm) {
        return protoMap.find(norm) != protoMap.end();
    }
};

std::unordered_map<std::string, ProtoClass*> ClassTable::protoMap;

int ShallowAutoLoadImpl(const char* /*cls*/) { return 1; }

int DeepAutoLoadImpl(const char* cls) {
    int status = ShallowAutoLoadImpl(cls);
    int result = 1;
    if (status) {
        static std::set<std::string> gClassOnStack;
        auto insertResult = gClassOnStack.insert(cls);
        if (insertResult.second) {
            ProtoClass* proto = ClassTable::GetProto(cls);
            if (proto) {
                for (const auto& element : proto->GetData()) {
                    const char* subtypename = element.typeName.c_str();
                    if (!element.isBasic && !ClassTable::GetDictNorm(subtypename)) {
                        result += DeepAutoLoadImpl(subtypename);
                    }
                }
            }
            gClassOnStack.erase(insertResult.first);
        }
    }
    return result;
}

int run(int iterations, int numClasses) {
    ClassTable::protoMap.clear();
    std::vector<std::string> names;
    names.reserve(numClasses);
    for (int i = 0; i < numClasses; ++i) {
        std::string name = "Class" + std::to_string(i);
        ProtoClass* proto = new ProtoClass;
        proto->data.reserve(10);
        for (int j = 0; j < 10; ++j) {
            proto->data.push_back({"Sub" + std::to_string(i) + '_' + std::to_string(j), false});
        }
        ClassTable::Register(name, proto);
        names.push_back(ClassTable::Normalize(name));
    }
    int sum = 0;
    for (int it = 0; it < iterations; ++it) {
        for (const auto& n : names) sum += DeepAutoLoadImpl(n.c_str());
    }
    return sum;
}

// Explicit template instantiation statements (if any) go here.
