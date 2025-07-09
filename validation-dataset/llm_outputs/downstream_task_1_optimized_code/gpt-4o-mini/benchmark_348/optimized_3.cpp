#include <array>
#include <set>
#include <shared_mutex>

constexpr int LAST_VALUETYPE = 32;
static std::set<int> EVTs;
static std::array<int, LAST_VALUETYPE> VTs{};
static std::shared_mutex VTMutex;

const int* getValueTypeList(int VT) {
    if (VT >= LAST_VALUETYPE) {
        std::unique_lock<std::shared_mutex> lock(VTMutex);
        auto it = EVTs.insert(VT).first;
        return &*it;
    } else {
        std::unique_lock<std::shared_mutex> lock(VTMutex);
        VTs[VT] = VT;
        return &VTs[VT];
    }
}

// Explicit template instantiation statements
// NOTE: Add actual template instantiation here if present