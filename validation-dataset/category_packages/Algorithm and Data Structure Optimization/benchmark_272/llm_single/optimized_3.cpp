#include <vector>
#include <cmath>
#include <functional>

static double brent_bisection(const std::function<double(double)>& f, double lower, double upper, int iterations = 100) {
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

void bootstrap(const std::vector<double>& times, const std::vector<double>& prices, std::vector<double>& rates) {
    const size_t n = times.size();
    rates.resize(n);
    std::vector<std::function<double(double)>> errors(n);
    for (size_t i = 0; i < n; ++i) {
        errors[i] = [i, &times, &prices](double r) { return std::exp(-r * times[i]) - prices[i]; };
    }
    for (size_t i = 0; i < n; ++i) {
        rates[i] = brent_bisection(errors[i], 0.0, 1.0);
    }
}

// Explicit template instantiation statements (unchanged) go here.
