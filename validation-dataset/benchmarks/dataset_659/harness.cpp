#include "ldap.hpp"
#include <chrono>
#include <iostream>
#include <string>
#include <vector>

std::vector<std::string> queryAttributes(LdapOperation& operation,
                                         const std::string& dn,
                                         const std::string& attribute,
                                         const std::string& filter = "(objectclass=*)",
                                         int scope = 0);

static std::vector<LdapObject> makeObjects(std::size_t count) {
    std::vector<LdapObject> objs(count);
    for (std::size_t i = 0; i < count; ++i) {
        LdapObject obj;
        obj.attrs["CN"] = {"value" + std::to_string(i)};
        // add several unrelated attributes to increase lookup cost
        for (int j = 0; j < 8; ++j) {
            obj.attrs["attr" + std::to_string(j)] = {std::to_string(j)};
        }
        objs[i] = std::move(obj);
    }
    return objs;
}

int main(int argc, char* argv[]) {
    std::string mode = "perf";
    std::size_t entries = 5000;
    int repeat = 5000;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--mode=correct") mode = "correct";
        else if (arg == "--mode=perf") mode = "perf";
        else if (arg.rfind("--entries=",0)==0) entries = std::stoull(arg.substr(10));
        else if (arg.rfind("--repeat=",0)==0) repeat = std::stoi(arg.substr(9));
    }

    if (mode == "correct") {
        std::vector<LdapObject> objs = makeObjects(3);
        LdapOperation op; op.set_results(objs);
        auto result = queryAttributes(op, "dn", "cn");
        std::size_t sum = 0;
        for (const auto& s : result) sum += s.size();
        std::cout << sum << "\n";
        return 0;
    }

    using clock = std::chrono::high_resolution_clock;
    long long dummy = 0;
    auto t1 = clock::now();
    std::vector<LdapObject> objs = makeObjects(entries);
    LdapOperation op;
    for (int r = 0; r < repeat; ++r) {
        op.set_results(objs);
        auto res = queryAttributes(op, "dn", "cn");
        dummy += res.size();
    }
    auto t2 = clock::now();
    std::cout << "Time: "
              << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()
              << " ms\n";
    if (dummy == 42) std::cerr << "";
    return 0;
}
