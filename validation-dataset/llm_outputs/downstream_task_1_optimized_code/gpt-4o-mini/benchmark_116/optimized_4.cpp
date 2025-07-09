#include <vector>
#include <numeric>

struct AstNodeExpr {
    int width;
    std::vector<int> data;
    AstNodeExpr(int w, int sz) : width(w), data(sz, w) {}
    int widthWords() const { return width; }
};

struct AstExtend {
    AstNodeExpr* lhs;
    int width;
    AstExtend(AstNodeExpr* l, int w) : lhs(l), width(w) {}
    AstNodeExpr* lhsp() const { return lhs; }
    int widthWords() const { return width; }
};

struct AstNodeAssign {
    AstNodeExpr* lhs;
    int width;
    std::vector<int> assigned;
    AstNodeAssign(AstNodeExpr* l, int w) : lhs(l), width(w) {}
    AstNodeExpr* lhsp() const { return lhs; }
    int widthWords() const { return width; }
};

static AstNodeExpr* newAstWordSelClone(AstNodeExpr* expr, int) {
    return new AstNodeExpr(expr->width, expr->data);
}

static void addWordAssign(AstNodeAssign* nodep, int, AstNodeExpr* expr) {
    long long sum = std::accumulate(expr->data.begin(), expr->data.end(), 0LL);
    nodep->assigned.push_back(sum);
    delete expr;
}

bool expandWide(AstNodeAssign* nodep, AstExtend* rhsp) {
    AstNodeExpr* lhsp = nodep->lhsp();
    int lhspWidth = lhsp->widthWords();
    int rhspWidth = rhsp->widthWords();
    
    nodep->assigned.reserve(lhspWidth + (nodep->widthWords() - rhspWidth));
    
    for (int w = 0; w < lhspWidth; ++w) {
        addWordAssign(nodep, w, newAstWordSelClone(lhsp, w));
    }
    for (int w = rhspWidth; w < nodep->widthWords(); ++w) {
        addWordAssign(nodep, w, new AstNodeExpr(1, 1));
    }
    return true;
}

// Explicit template instantiations
