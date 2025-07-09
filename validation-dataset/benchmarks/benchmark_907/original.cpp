#include "liveinterval.hpp"
#include <vector>
#include <cmath>

uint64_t run(const std::vector<MachineBasicBlock>& func, int repeat) {
    std::vector<float> weights(64, 0.0f);
    uint64_t sum = 0;
    for (int r = 0; r < repeat; ++r) {
        for (auto mbbi = func.begin(), mbbe = func.end(); mbbi != mbbe; ++mbbi) {
            unsigned loopDepth = mbbi->loopDepth;
            for (auto mii = mbbi->begin(), mie = mbbi->end(); mii != mie; ) {
                for (unsigned i = 0; i < mii->getNumOperands(); ++i) {
                    const MachineOperand& mop = mii->getOperand(i);
                    if (mop.isRegister() && mop.getReg() && isVirtualRegister(mop.getReg())) {
                        unsigned reg = rep(mop.getReg());
                        weights[reg] += (mop.isUse() + mop.isDef()) * std::pow(10.0f, static_cast<float>(loopDepth));
                        sum += static_cast<uint64_t>(weights[reg]);
                    }
                }
                ++mii;
            }
        }
    }
    return sum;
}
