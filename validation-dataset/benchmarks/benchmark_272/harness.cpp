#include <vector>
#include <iostream>
#include <chrono>
#include <string>
#include <cmath>

void bootstrap(const std::vector<double>& times,
               const std::vector<double>& prices,
               std::vector<double>& rates);

std::vector<double> create_times(size_t n) {
    std::vector<double> t(n);
    for (size_t i = 0; i < n; ++i)
        t[i] = 0.05 * (i + 1); // increasing times
    return t;
}

std::vector<double> create_prices(const std::vector<double>& times) {
    std::vector<double> p(times.size());
    for (size_t i = 0; i < times.size(); ++i)
        p[i] = std::exp(-0.05 * times[i]);
    return p;
}

int main(int argc, char* argv[]) {
    std::string mode = "perf";
    size_t instruments = 400;
    int repeat = 12000;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--mode=correct")
            mode = "correct";
        else if (arg == "--mode=perf")
            mode = "perf";
        else if (arg.rfind("--inst=", 0) == 0)
            instruments = std::stoul(arg.substr(7));
        else if (arg.rfind("--repeat=", 0) == 0)
            repeat = std::stoi(arg.substr(9));
    }

    auto times = create_times(instruments);
    auto prices = create_prices(times);
    std::vector<double> rates;

    if (mode == "correct") {
        bootstrap(times, prices, rates);
        double sum = 0.0;
        for (double r : rates) sum += r;
        std::cout << sum << '\n';
    } else {
        using clock = std::chrono::high_resolution_clock;

        auto t1 = clock::now();
        for (int i = 0; i < repeat; ++i)
            bootstrap(times, prices, rates);
        auto t2 = clock::now();

        std::cout << "Time: " << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count() << " ms\n";
    }

    return 0;
}
