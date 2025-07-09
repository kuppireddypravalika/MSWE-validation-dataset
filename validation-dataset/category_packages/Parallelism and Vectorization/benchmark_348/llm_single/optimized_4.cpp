#include <array>
#include <set>
#include <mutex>

constexpr int LAST_VALUETYPE = 32;
static std::set<int> EVTs;
static std::array<int, LAST_VALUETYPE> VTs{};
static std::mutex VTMutex;

const int* getValueTypeList(int VT) {
    if (VT >= LAST_VALUETYPE) {
        std::lock_guard<std::mutex> lock(VTMutex);
        return &*EVTs.insert(VT).first;
    } else {
        VTs[VT] = VT;
        return &VTs[VT];
    }
}

// Explicit template instantiation (unchanged)
