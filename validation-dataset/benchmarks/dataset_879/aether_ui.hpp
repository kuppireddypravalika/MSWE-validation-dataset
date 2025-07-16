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
