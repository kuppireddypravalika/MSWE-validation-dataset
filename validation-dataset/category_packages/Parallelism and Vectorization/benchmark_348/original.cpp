#include <array>
#include <mutex>
#include <set>

constexpr int LAST_VALUETYPE = 32;
static std::set<int> EVTs;
static std::array<int, LAST_VALUETYPE> VTs{};
static std::mutex VTMutex;

// Mimics the original locked implementation
const int* getValueTypeList(int VT) {
    std::lock_guard<std::mutex> lock(VTMutex);
    if (VT >= LAST_VALUETYPE) {
        auto it = EVTs.insert(VT).first;
        return &*it;
    } else {
        VTs[VT] = VT;
        return &VTs[VT];
    }
}
