#include <array>
#include <set>
#include <atomic>
#include <mutex>

constexpr int LAST_VALUETYPE = 32;
static std::set<int> EVTs;
static std::array<int, LAST_VALUETYPE> VTs{};
static std::mutex VTMutex;

const int* getValueTypeList(int VT) {
    if (VT >= LAST_VALUETYPE) {
        std::lock_guard<std::mutex> lock(VTMutex);
        auto it = EVTs.insert(VT).first;
        return &*it;
    } else {
        VTs[VT] = VT;
        return &VTs[VT];
    }
}

// Explicit template instantiation statements if any go here.