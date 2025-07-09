void hoist(Instruction& I, BasicBlock& Preheader, AliasSetTracker& CurAST) {
    // Remove from current block but keep instruction alive.
    I.removeFromParent();
    // Insert into preheader before terminator.
    Preheader.insertBefore(Preheader.terminator, &I);
}
