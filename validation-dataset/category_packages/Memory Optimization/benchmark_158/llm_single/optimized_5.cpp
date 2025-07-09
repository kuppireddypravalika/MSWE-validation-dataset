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
    /* Unrolling the loop for improved performance */
    for (unsigned i = 0; i < iters; i += 4) {
        sink += src + dst + i;
        sink += src + dst + i + 1;
        sink += src + dst + i + 2;
        sink += src + dst + i + 3;
    }
}

unsigned run_mri(MachineRegisterInfo& mri) {
    MachineBasicBlock mbb;
    unsigned e = mri.LiveIns.size();
    for (unsigned i = 0; i < e; ++i) {
        if (mri.LiveIns[i].second) {
            if (!use_empty(mri, mri.LiveIns[i].second)) {
                heavy_copy(mri.LiveIns[i].first, mri.LiveIns[i].second, mri.workload);
            }
        }
        mbb.addLiveIn(mri.LiveIns[i].first);
    }
    return 0; // results are irrelevant
}

unsigned run(MachineRegisterInfo& mri) {
    return run_original(mri);
}
