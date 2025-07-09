void hoist(Instruction& I, BasicBlock& Preheader, AliasSetTracker& CurAST) {
    // Keep the instruction alive and remove it from the current block.
    BasicBlock* currentBlock = I.parent;
    currentBlock->removeInstruction(&I);
    // Insert into the preheader before the terminator efficiently.
    Preheader.insertBefore(Preheader.getTerminator(), &I);
}
