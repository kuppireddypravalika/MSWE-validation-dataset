#include <vector>
#include <cmath>
#include <functional>

static double brent_bisection(const std::function<double(double)>& f,
                              double lower,
                              double upper,
                              int iterations = 100) {
    double a = lower;
    double b = upper;
    double fa = f(a);
    double fb = f(b);
    for (int i = 0; i < iterations; ++i) {
        double mid = 0.5 * (a + b);
        double fm = f(mid);
        if (fa * fm <= 0) {
            b = mid;
            fb = fm;
        } else {
            a = mid;
            fa = fm;
        }
    }
    return 0.5 * (a + b);
}

void bootstrap(const std::vector<double>& times,
               const std::vector<double>& prices,
               std::vector<double>& rates) {
    const size_t n = times.size();
    rates.resize(n);
    for (size_t i = 0; i < n; ++i) {
        double price = prices[i];
        double time = times[i];
        auto error = [price, time](double r) { return std::exp(-r * time) - price; };
        rates[i] = brent_bisection(error, 0.0, 1.0);
    }
}

// Template instantiation statements (if any) should be copied here unchanged.