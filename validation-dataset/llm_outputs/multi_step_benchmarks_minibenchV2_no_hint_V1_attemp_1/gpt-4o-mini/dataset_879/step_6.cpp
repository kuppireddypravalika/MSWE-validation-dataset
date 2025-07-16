// aether_ui.hpp
#pragma once
#include <vector>
#include <chrono>
#include <algorithm>

inline float lerp(float a, float b, float t) { return a + (b - a) * t; }

namespace Aether {
struct PeakInfos {
    std::vector<float> peaks;
};

class UI {
public:
    class View {
    public:
        PeakInfos peak_infos;
        std::vector<float> parameters;
        std::chrono::steady_clock::time_point last_frame;

        explicit View(size_t n)
            : peak_infos{std::vector<float>(n, 0.f)},
              parameters(51 + n, 0.f),
              last_frame(std::chrono::steady_clock::now()) {}

        float get_parameter(size_t idx) const { return parameters[idx]; }
        void parameter_update(size_t idx, float v) { parameters[idx] = v; }
        void add_peaks(const std::vector<float>& peaks) { peak_infos.peaks = peaks; }

        void update_peaks_original();
        void update_peaks_optimized();
    };
};
} // namespace Aether

// original.cpp
#include "aether_ui.hpp"

namespace Aether {
void UI::View::update_peaks_optimized() {
    const auto current_time = std::chrono::steady_clock::now();
    const float dt = 0.000001f * std::chrono::duration_cast<std::chrono::microseconds>(current_time - last_frame).count();

    // Allocate space for old parameter values to avoid multiple get_parameter calls.
    size_t peak_size = peak_infos.peaks.size();
    std::vector<float> old_values(peak_size);

    // Retrieve old parameter values in a single pass
    for (size_t i = 0; i < peak_size; ++i) {
        old_values[i] = get_parameter(51 + i);
    }

    for (size_t i = 0; i < peak_size; ++i) {
        float old_value = old_values[i];
        float target_peak = peak_infos.peaks[i];
        float lerp_factor = (old_value < target_peak) ? std::min(8.f * dt, 1.f) : std::min(2.f * dt, 1.f);
        parameter_update(51 + i, lerp(old_value, target_peak, lerp_factor));
    }
    last_frame = current_time;
}
} // namespace Aether

extern "C" double run(int repeat, size_t size) {
    using namespace Aether;
    UI::View view(size);
    std::vector<float> peaks(size, 0.5f);
    double result = 0.0;
    for (int r = 0; r < repeat; ++r) {
        view.last_frame = std::chrono::steady_clock::now() - std::chrono::milliseconds(16);
        view.add_peaks(peaks);
        view.update_peaks_optimized();
        result += view.parameters[51];
    }
    return result;
}

// Explicit template instantiations, if any, should go here.