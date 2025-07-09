#include "licm_sim.hpp"

void hoist(Instruction& I, BasicBlock& Preheader, AliasSetTracker& CurAST) {
    // Remove from current block but keep instruction alive.
    I.removeFromParent();
    // Insert into preheader before terminator.
    Preheader.addInstruction(&I);
    Preheader.insts.splice(Preheader.terminator->it, Preheader.insts, --Preheader.insts.end());
}

// explicit instantiation helper so the functions are emitted