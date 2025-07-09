void hoist(Instruction& I, BasicBlock& Preheader, AliasSetTracker& CurAST) {
    // Move the instruction directly to the preheader's tail,
    // preserving its lifetime and leaving the original block intact.
    I.removeFromParent();
    Preheader.insts.insert(std::prev(Preheader.insts.end()), &I);
    I.parent = &Preheader;
    I.it = std::prev(Preheader.insts.end());
}
