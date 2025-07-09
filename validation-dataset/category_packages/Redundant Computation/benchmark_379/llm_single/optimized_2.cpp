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
        std::string norm = name;
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
        auto it = protoMap.find(Normalize(name));
        return it == protoMap.end() ? nullptr : it->second;
    }

    static ProtoClass* GetProtoNorm(const char* norm) {
        return protoMap.count(norm) ? protoMap[norm] : nullptr;
    }

    static bool GetDictNorm(const char* norm) {
        return protoMap.count(norm) > 0;
    }
};

std::unordered_map<std::string, ProtoClass*> ClassTable::protoMap;

int ShallowAutoLoadImpl(const char* /*cls*/) { return 1; }

int DeepAutoLoadImpl(const char* cls) {
    int status = ShallowAutoLoadImpl(cls);
    int result = 1;
    if (status) {
        static std::set<std::string> gClassOnStack;
        // Use emplace to avoid unnecessary copy of string
        auto insertResult = gClassOnStack.emplace(cls).second;
        if (insertResult) {
            ProtoClass* proto = ClassTable::GetProto(cls);
            if (proto) {
                for (const auto& element : proto->GetData()) {
                    if (!element.isBasic && !ClassTable::GetDictNorm(element.typeName.c_str())) {
                        result += DeepAutoLoadImpl(element.typeName.c_str());
                    }
                }
            }
            gClassOnStack.erase(cls);
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

// Explicit template instantiation statements
