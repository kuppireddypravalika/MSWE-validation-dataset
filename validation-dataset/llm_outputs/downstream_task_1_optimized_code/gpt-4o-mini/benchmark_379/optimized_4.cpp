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
        std::string norm;
        norm.reserve(name.size()); // Reserve space upfront
        for (char c : name) {
            if (c != ' ') norm.push_back(std::tolower(static_cast<unsigned char>(c)));
        }
        return norm;
    }

    static void Register(const std::string& name, ProtoClass* proto) {
        protoMap[Normalize(name)] = proto;
    }

    static ProtoClass* GetProto(const char* name) {
        return GetProtoNorm(Normalize(name).c_str()); // Use normalized version directly
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
        std::string clsStr(cls);
        if (gClassOnStack.insert(clsStr).second) {
            ProtoClass* proto = ClassTable::GetProto(cls);
            if (proto) {
                for (const auto& element : proto->GetData()) {
                    if (!element.isBasic) {
                        if (!ClassTable::GetDictNorm(element.typeName.c_str())) {
                            result += DeepAutoLoadImpl(element.typeName.c_str());
                        }
                    }
                }
            }
            gClassOnStack.erase(clsStr);
        }
    }
    return result;
}

// Explicit template instantiations
