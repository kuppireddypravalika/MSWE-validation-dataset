#include <vector>
#include <complex>
#include <cmath>
#include <cstdint>
#include <unordered_map>
#include <memory>
#include <mutex>

struct FFTPlanOptimized {
    uint32_t size;
};

static std::mutex fft_mutex;
static std::unordered_map<uint32_t, std::unique_ptr<FFTPlanOptimized>> fft_plans;

FFTPlanOptimized* prepareFFT(uint32_t fftSize) {
    std::unique_lock<std::mutex> lock(fft_mutex);
    auto it = fft_plans.find(fftSize);
    if (it != fft_plans.end())
        return it->second.get();

    auto plan = std::make_unique<FFTPlanOptimized>();
    plan->size = fftSize;
    fft_plans[fftSize] = std::move(plan);
    return fft_plans[fftSize].get();
}

static void execute_fft(const FFTPlanOptimized* plan, const std::vector<double>& in, std::vector<std::complex<double>>& out) {
    const uint32_t N = plan->size;
    out.resize(N / 2 + 1);
    for (uint32_t k = 0; k <= N / 2; ++k) {
        double real = 0.0;
        double imag = 0.0;
        for (uint32_t n = 0; n < N; ++n) {
            const double angle = -2.0 * M_PI * k * n / N;
            const double cos_angle = std::cos(angle);
            const double sin_angle = std::sin(angle);
            real += in[n] * cos_angle;
            imag += in[n] * sin_angle;
        }
        out[k] = {real, imag};
    }
}

class STFT {
public:
    STFT(uint32_t size) : fftSize(size), window(size), real(size), cpx(size / 2 + 1) {
        plan = prepareFFT(size);
        for (uint32_t i = 0; i < size; ++i)
            window[i] = 0.5 * (1.0 - std::cos(2.0 * M_PI * i / (size - 1)));
    }

    double process(const std::vector<double>& input) {
        for (uint32_t i = 0; i < fftSize; ++i)
            real[i] = window[i] * input[i];
        execute_fft(plan, real, cpx);
        double sum = 0.0;
        for (const auto& v : cpx) sum += std::abs(v);
        return sum;
    }

private:
    FFTPlanOptimized* plan;
    const uint32_t fftSize;
    std::vector<double> window;
    std::vector<double> real;
    std::vector<std::complex<double>> cpx;
};

extern "C" double run(STFT* stft, const std::vector<double>& in) {
    return stft->process(in);
}
extern "C" STFT* create(uint32_t size) { return new STFT(size); }
extern "C" void destroy(STFT* stft) { delete stft; }

// Keep original explicit template instantiation statements if there are any.
