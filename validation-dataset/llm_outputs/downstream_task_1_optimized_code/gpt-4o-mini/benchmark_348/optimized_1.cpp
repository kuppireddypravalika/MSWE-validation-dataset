#include <array>
#include <set>
#include <mutex>

constexpr int LAST_VALUETYPE = 32;
static std::set<int> EVTs;
static std::array<int, LAST_VALUETYPE> VTs{};
static std::mutex VTMutex;

// Optimized implementation
const int* getValueTypeList(int VT) {
    // Use a lock on the critical section only when required
    if (VT >= LAST_VALUETYPE) {
        std::lock_guard<std::mutex> lock(VTMutex);
        auto it = EVTs.insert(VT).first;
        return &*it;
    } else {
        VTs[VT] = VT;
        return &VTs[VT];
    }
}

// Reproduced explicit template instantiation statements unchanged
