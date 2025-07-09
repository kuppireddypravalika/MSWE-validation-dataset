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
    // Reduce the heavy copy overhead by using the std::vector copy constructor directly
t    return new AstNodeExpr(expr->width, expr->data);
}

static void addWordAssign(AstNodeAssign* nodep, int, AstNodeExpr* expr) {
    long long sum = std::accumulate(expr->data.begin(), expr->data.end(), 0LL);
    nodep->assigned.push_back(sum);
    delete expr;
}

bool expandWide(AstNodeAssign* nodep, AstExtend* rhsp) {
    AstNodeExpr* lhsp = nodep->lhsp();
    int lhspWidthWords = lhsp->widthWords();
    int rhspWidthWords = rhsp->widthWords();

    for (int w = 0; w < lhspWidthWords; ++w) {
        addWordAssign(nodep, w, newAstWordSelClone(lhsp, w));
    }

    for (int w = rhspWidthWords; w < nodep->widthWords(); ++w) {
        addWordAssign(nodep, w, new AstNodeExpr(1, 1));
    }

    return true;
}

// Explicit template instantiation statements unchanged
