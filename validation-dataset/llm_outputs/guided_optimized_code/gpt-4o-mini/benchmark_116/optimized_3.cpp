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
    AstNodeExpr* c = new AstNodeExpr(expr->width, expr->data.size());
    c->data = expr->data; // heavy copy
    return c;
}

static void addWordAssign(AstNodeAssign* nodep, AstNodeExpr* expr) {
    long long sum = std::accumulate(expr->data.begin(), expr->data.end(), 0LL);
    nodep->assigned.push_back(sum);
    delete expr;
}

static void assignFromLHS(AstNodeAssign* nodep) {
    AstNodeExpr* lhsp = nodep->lhsp();
    for (int w = 0; w < lhsp->widthWords(); ++w) {
        addWordAssign(nodep, newAstWordSelClone(lhsp, w));
    }
}

static void fillExtraAssignments(AstNodeAssign* nodep, int startWidth) {
    for (int w = startWidth; w < nodep->widthWords(); ++w) {
        addWordAssign(nodep, new AstNodeExpr(1, 1));
    }
}

bool expandWide(AstNodeAssign* nodep, AstExtend* rhsp) {
    assignFromLHS(nodep);
    fillExtraAssignments(nodep, rhsp->widthWords());
    return true;
}

// Explicit template instantiation statements would be added here if applicable.