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
    const auto& uses = mri.Uses[v];
    for (const Use& u : uses) {
        if (!u.isDebug) return false;
    }
    return true;
}

static inline void heavy_copy(unsigned src, unsigned dst, unsigned iters) {
    for (unsigned i = 0; i < iters; ++i) {
        sink += src + dst + i;
    }
}

unsigned run_mri(MachineRegisterInfo& mri) {
    MachineBasicBlock mbb;
    const unsigned numLiveIns = mri.LiveIns.size();
    mbb.ins.reserve(numLiveIns); // Reserve space in advance to avoid multiple reallocations
    
    for (const auto& liveIn : mri.LiveIns) {
        unsigned first = liveIn.first;
        unsigned second = liveIn.second;
        if (second) {
            if (!use_empty(mri, second)) {
                heavy_copy(first, second, mri.workload);
                mbb.addLiveIn(first);
            }
        } else {
            mbb.addLiveIn(first);
        }
    }
    return 0; // results are irrelevant
}

unsigned run(MachineRegisterInfo& mri) {
    return run_original(mri);
}
