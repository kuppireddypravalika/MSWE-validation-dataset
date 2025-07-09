#include <vector>
#include <cmath>
#include <functional>

static double newton_safe(const std::function<double(double)>& f,
                          double guess,
                          double lower,
                          double upper,
                          int iterations = 20) {
    double x = guess;
    for (int i = 0; i < iterations; ++i) {
        double fx = f(x);
        if (std::fabs(fx) < 1e-12)
            return x;
        double eps = 1e-5;
        double dfx = (f(x + eps) - f(x - eps)) / (2 * eps);
        if (dfx == 0.0) {
            x = 0.5 * (lower + upper);
        } else {
            double xn = x - fx / dfx;
            if (xn < lower || xn > upper)
                xn = 0.5 * (lower + upper);
            x = xn;
        }
    }
    return x;
}

void bootstrap(const std::vector<double>& times,
               const std::vector<double>& prices,
               std::vector<double>& rates) {
    const size_t n = times.size();
    rates.resize(n);
    for (size_t i = 0; i < n; ++i) {
        auto error = [&](double r) { return std::exp(-r * times[i]) - prices[i]; };
        rates[i] = newton_safe(error, 0.05, 0.0, 1.0);
    }
}
