#include "aether_ui.hpp"

namespace Aether {
void UI::View::update_peaks_optimized() {
    using namespace std::chrono;
    const float dt = 0.000001f * duration_cast<microseconds>(steady_clock::now() - last_frame).count();
    for (size_t i = 0; i < peak_infos.peaks.size(); ++i) {
        float old_value = get_parameter(51 + i);
        if (old_value < peak_infos.peaks[i])
            parameter_update(51 + i, lerp(old_value, peak_infos.peaks[i], std::min(8.f * dt, 1.f)));
        else
            parameter_update(51 + i, lerp(old_value, peak_infos.peaks[i], std::min(2.f * dt, 1.f)));
    }
    last_frame = std::chrono::steady_clock::now();
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
