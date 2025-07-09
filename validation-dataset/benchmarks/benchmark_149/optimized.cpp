#include "licm_sim.hpp"

void hoist(Instruction& I, BasicBlock& Preheader, AliasSetTracker& CurAST) {
    // Instruction leaves the loop, remove from alias set tracker.
    CurAST.deleteValue(&I);
    // Move instruction directly before preheader terminator.
    I.moveBefore(Preheader.getTerminator());
}

