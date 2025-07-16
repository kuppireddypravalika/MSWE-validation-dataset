#pragma once
#include <algorithm>
#include <string>
#include <unordered_map>
#include <vector>

// Minimal stand-ins for Qt/KLDAP classes used in the benchmark
class LdapObject {
public:
    using AttrMap = std::unordered_map<std::string, std::vector<std::string>>;

    AttrMap attrs;

    const AttrMap& attributes() const { return attrs; }

    std::vector<std::string> values(const std::string& attr) const {
        auto it = attrs.find(attr);
        if (it != attrs.end()) return it->second;
        // fallback to case-insensitive match
        std::string lower = to_lower(attr);
        for (const auto& kv : attrs) {
            if (to_lower(kv.first) == lower) return kv.second;
        }
        return {};
    }

private:
    static std::string to_lower(const std::string& s) {
        std::string r = s;
        std::transform(r.begin(), r.end(), r.begin(), [](unsigned char c){ return std::tolower(c); });
        return r;
    }
};

class LdapOperation {
public:
    enum { RES_SEARCH_ENTRY = 1, RES_DONE = 0 };

    void set_results(const std::vector<LdapObject>& r) {
        results = r;
        idx = 0;
    }

    void reset() { idx = 0; }

    int search(const std::string&, int, const std::string&, const std::vector<std::string>&) {
        idx = 0;
        return 0; // id
    }

    int waitForResult(int, int) {
        if (idx < results.size()) {
            current = results[idx++];
            return RES_SEARCH_ENTRY;
        }
        return RES_DONE;
    }

    const LdapObject& object() const { return current; }

private:
    std::vector<LdapObject> results;
    std::size_t idx = 0;
    LdapObject current;
};
