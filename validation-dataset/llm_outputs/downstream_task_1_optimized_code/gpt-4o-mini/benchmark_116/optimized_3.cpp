bool expandWide(AstNodeAssign* nodep, AstExtend* rhsp) {
    AstNodeExpr* lhsp = nodep->lhsp();
    const int lhspWidthWords = lhsp->widthWords();
    const int rhspWidthWords = rhsp->widthWords();
    const int nodepWidthWords = nodep->widthWords();

    // Preallocate the assigned vector for performance
    nodep->assigned.reserve(nodepWidthWords);

    // Clone and sum for the left-hand side expression
    for (int w = 0; w < lhspWidthWords; ++w) {
        AstNodeExpr* clonedExpr = newAstWordSelClone(lhsp, w);
        addWordAssign(nodep, w, clonedExpr);
    }

    // If the right-hand side width is less than the node width, we add extra default assignments
    for (int w = rhspWidthWords; w < nodepWidthWords; ++w) {
        addWordAssign(nodep, w, new AstNodeExpr(1, 1));
    }
    return true;
}