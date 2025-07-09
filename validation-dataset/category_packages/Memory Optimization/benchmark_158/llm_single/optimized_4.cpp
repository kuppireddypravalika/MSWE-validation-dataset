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
    unsigned long long temp_sink = 0; // Use a local variable to reduce memory access overhead
    for (unsigned i = 0; i < iters; ++i) {
        temp_sink += src + dst + i;
    }
    sink += temp_sink; // Write back once after the loop
}

unsigned run_mri(MachineRegisterInfo& mri) {
    MachineBasicBlock mbb;
    size_t e = mri.LiveIns.size();
    for (size_t i = 0; i < e; ++i) {
        unsigned first = mri.LiveIns[i].first;
        unsigned second = mri.LiveIns[i].second;
        // Process the second LiveIn only if it's greater than 0
        if (second) {
            if (!use_empty(mri, second)) {
                heavy_copy(first, second, mri.workload);
                mbb.addLiveIn(first);
            }
        }
        mbb.addLiveIn(first); // Always add first
    }
    return 0; // results are irrelevant
}

unsigned run(MachineRegisterInfo& mri) {
    return run_mri(mri);
}
