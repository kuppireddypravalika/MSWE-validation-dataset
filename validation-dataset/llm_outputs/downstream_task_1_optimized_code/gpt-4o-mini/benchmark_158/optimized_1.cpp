#include <vector>
#include <cstddef>

struct Use { bool isDebug; };

struct MachineBasicBlock {
    std::vector<unsigned> ins;
    void addLiveIn(unsigned reg) { ins.push_back(reg); }
};

struct MachineRegisterInfo {
    struct LiveInPair { unsigned first; unsigned second; };
    std::vector<LiveInPair> LiveIns;
    std::vector<std::vector<Use>> Uses;
    unsigned workload;
};

extern volatile unsigned long long sink;

static inline bool use_empty(const MachineRegisterInfo& mri, unsigned v) {
    return mri.Uses[v].empty();
}

static inline bool use_nodbg_empty(const MachineRegisterInfo& mri, unsigned v) {
    for (const auto& u : mri.Uses[v]) if (!u.isDebug) return false;
    return true;
}

static inline void heavy_copy(unsigned src, unsigned dst, unsigned iters) {
    unsigned long long temp = sink; // Fetch the value once to minimize cache misses.
    for (unsigned i = 0; i < iters; ++i) {
        temp += src + dst + i;
    }
    sink = temp; // Store back the modified value in one go.
}

unsigned run_mri(MachineRegisterInfo& mri) {
    MachineBasicBlock mbb;
    const auto& liveIns = mri.LiveIns;
    size_t size = liveIns.size(); // Cache size to avoid multiple calls.
    for (size_t i = 0; i < size; ++i) {
        const auto& liveIn = liveIns[i];
        if (liveIn.second) {
            if (!use_empty(mri, liveIn.second)) {
                heavy_copy(liveIn.first, liveIn.second, mri.workload);
                mbb.addLiveIn(liveIn.first);
            }
        } else {
            mbb.addLiveIn(liveIn.first);
        }
    }
    return 0; // results are irrelevant
}

unsigned run(MachineRegisterInfo& mri) {
    return run_mri(mri);
}
