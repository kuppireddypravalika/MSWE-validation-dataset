#include <vector>
#include <complex>
#include <cmath>
#include <cstdint>
#include <unordered_map>
#include <memory>
#include <mutex>
#include <algorithm>

struct FFTPlanOrig {
    uint32_t size;
};

static std::mutex fft_mutex_orig;
static std::unordered_map<uint32_t, std::unique_ptr<FFTPlanOrig>> fft_plans_orig;

FFTPlanOrig* prepareFFT_original(uint32_t fftSize) {
    std::unique_lock<std::mutex> lock(fft_mutex_orig);
    auto it = fft_plans_orig.find(fftSize);
    if (it != fft_plans_orig.end())
        return it->second.get();

    auto plan = std::make_unique<FFTPlanOrig>();
    plan->size = fftSize;
    fft_plans_orig[fftSize] = std::move(plan);
    return fft_plans_orig[fftSize].get();
}

static void execute_fft_original(const FFTPlanOrig* plan,
                                 const std::vector<double>& in,
                                 std::vector<std::complex<double>>& out) {
    uint32_t N = plan->size;
    out.resize(N / 2 + 1);
    std::vector<double> cos_table(N);
    std::vector<double> sin_table(N);
    for (uint32_t n = 0; n < N; ++n) {
        cos_table[n] = std::cos(-2.0 * M_PI * n / N);
        sin_table[n] = std::sin(-2.0 * M_PI * n / N);
    }
    for (uint32_t k = 0; k <= N / 2; ++k) {
        double real = 0.0;
        double imag = 0.0;
        for (uint32_t n = 0; n < N; ++n) {
            real += in[n] * cos_table[(k * n) % N];
            imag += in[n] * sin_table[(k * n) % N];
        }
        out[k] = {real, imag};
    }
}

class STFT {
public:
    STFT(uint32_t size) {
        plan = prepareFFT_original(size);
        fftSize = size;
        window.resize(size);
        real.resize(size);
        for (uint32_t i = 0; i < size; ++i)
            window[i] = 0.5 * (1.0 - std::cos(2.0 * M_PI * i / (size - 1)));
    }

    double process(const std::vector<double>& input) {
        for (uint32_t i = 0; i < fftSize; ++i)
            real[i] = window[i] * input[i];
        execute_fft_original(plan, real, cpx);
        return std::accumulate(cpx.begin(), cpx.end(), 0.0, [](double sum, const std::complex<double>& v) {
            return sum + std::abs(v);
        });
    }

private:
    FFTPlanOrig* plan;
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
