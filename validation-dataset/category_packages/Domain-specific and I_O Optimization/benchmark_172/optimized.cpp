#include <vector>
#include <complex>
#include <cmath>
#include <cstdint>
#include <unordered_map>
#include <memory>
#include <mutex>

struct FFTPlanOpt {
    uint32_t size;
    std::vector<double> cos_table;
    std::vector<double> sin_table;
};

static std::mutex fft_mutex_opt;
static std::unordered_map<uint32_t, std::unique_ptr<FFTPlanOpt>> fft_plans_opt;

FFTPlanOpt* prepareFFT_optimized(uint32_t fftSize) {
    std::unique_lock<std::mutex> lock(fft_mutex_opt);
    auto it = fft_plans_opt.find(fftSize);
    if (it != fft_plans_opt.end())
        return it->second.get();

    auto plan = std::make_unique<FFTPlanOpt>();
    plan->size = fftSize;
    plan->cos_table.resize((fftSize / 2 + 1) * fftSize);
    plan->sin_table.resize((fftSize / 2 + 1) * fftSize);
    for (uint32_t k = 0; k <= fftSize / 2; ++k) {
        for (uint32_t n = 0; n < fftSize; ++n) {
            double angle = -2.0 * M_PI * k * n / fftSize;
            plan->cos_table[k * fftSize + n] = std::cos(angle);
            plan->sin_table[k * fftSize + n] = std::sin(angle);
        }
    }
    fft_plans_opt[fftSize] = std::move(plan);
    return fft_plans_opt[fftSize].get();
}

static void execute_fft_optimized(const FFTPlanOpt* plan,
                                  const std::vector<double>& in,
                                  std::vector<std::complex<double>>& out) {
    uint32_t N = plan->size;
    out.resize(N / 2 + 1);
    for (uint32_t k = 0; k <= N / 2; ++k) {
        double real = 0.0;
        double imag = 0.0;
        const double* cosRow = &plan->cos_table[k * N];
        const double* sinRow = &plan->sin_table[k * N];
        for (uint32_t n = 0; n < N; ++n) {
            real += in[n] * cosRow[n];
            imag += in[n] * sinRow[n];
        }
        out[k] = {real, imag};
    }
}

class STFT {
public:
    STFT(uint32_t size) {
        plan = prepareFFT_optimized(size);
        fftSize = size;
        window.resize(size);
        real.resize(size);
        for (uint32_t i = 0; i < size; ++i)
            window[i] = 0.5 * (1.0 - std::cos(2.0 * M_PI * i / (size - 1)));
    }

    double process(const std::vector<double>& input) {
        for (uint32_t i = 0; i < fftSize; ++i)
            real[i] = window[i] * input[i];
        execute_fft_optimized(plan, real, cpx);
        double sum = 0.0;
        for (auto& v : cpx) sum += std::abs(v);
        return sum;
    }

private:
    FFTPlanOpt* plan;
    uint32_t fftSize;
    std::vector<double> window;
    std::vector<double> real;
    std::vector<std::complex<double>> cpx;
};

extern "C" double run(STFT* stft, const std::vector<double>& in) {
    return stft->process(in);
}
extern "C" STFT* create(uint32_t size) { return new STFT(size); }
extern "C" void destroy(STFT* stft) { delete stft; }
