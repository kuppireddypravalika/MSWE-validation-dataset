#include <array>
#include <set>
#include <atomic>

constexpr int LAST_VALUETYPE = 32;
static std::set<int> EVTs;
static std::array<int, LAST_VALUETYPE> VTs{};

// Lock-free version using a memory fence
const int* getValueTypeList(int VT) {
    if (VT >= LAST_VALUETYPE) {
        auto it = EVTs.insert(VT).first;
        return &*it;
    } else {
        VTs[VT] = VT;
        std::atomic_thread_fence(std::memory_order_seq_cst);
        return VTs.data() + VT;
    }
}
