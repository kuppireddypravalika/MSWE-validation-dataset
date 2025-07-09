void hoist(Instruction& I, BasicBlock& Preheader, AliasSetTracker& CurAST) {
    // Remove from current block but keep instruction alive.
    BasicBlock* currentBlock = I.parent;
    auto instructionIt = I.it;

    currentBlock->insts.erase(instructionIt);
    I.parent = &Preheader;
    Preheader.insts.insert(std::prev(Preheader.insts.end()), &I);
    I.it = std::prev(Preheader.insts.end());
}