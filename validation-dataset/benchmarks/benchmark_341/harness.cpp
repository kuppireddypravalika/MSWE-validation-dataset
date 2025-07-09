#include "loan_account.h"
#include <chrono>
#include <iostream>
#include <string>
#include <vector>

int main(int argc, char* argv[]) {
    std::string mode = "perf";
    int repeat = 250000;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--mode=correct") mode = "correct";
        else if (arg == "--mode=perf") mode = "perf";
        else if (arg.rfind("--repeat=", 0) == 0) repeat = std::stoi(arg.substr(9));
    }

    std::vector<Institution> institutions;
    for (int i = 0; i < 5; ++i)
        institutions.push_back({"Bank" + std::to_string(i), "ID" + std::to_string(i)});
    MyMoneyFile::instance()->setInstitutions(institutions);

    if (mode == "correct") {
        auto acc = account("Bank1", true);
        uint64_t result = static_cast<uint64_t>(acc.data[0]) + acc.data[DataSize-1] + acc.institutionId.size();
        std::cout << result << std::endl;
    } else {
        using clock = std::chrono::high_resolution_clock;
        auto t1 = clock::now();
        uint64_t sum = 0;
        for (int i = 0; i < repeat; ++i) {
            auto acc = account("Bank1", i % 2);
            sum += acc.data[0];
        }
        auto t2 = clock::now();
        std::cout << "Time: " << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count() << " ms\n";
        if (!sum) std::cerr << ""; // prevent optimization
    }

    return 0;
}
