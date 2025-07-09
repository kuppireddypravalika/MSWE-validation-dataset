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

static void addWordAssign(AstNodeAssign* nodep, int, AstNodeExpr* expr) {
    long long sum = 0;
    for (int v : expr->data) sum += v;
    nodep->assigned.push_back(sum);
    delete expr;
}

bool expandWide(AstNodeAssign* nodep, AstExtend* rhsp) {
    AstNodeExpr* rlhsp = rhsp->lhsp();
    for (int w = 0; w < rlhsp->widthWords(); ++w) {
        addWordAssign(nodep, w, newAstWordSelClone(rlhsp, w));
    }
    for (int w = rlhsp->widthWords(); w < nodep->widthWords(); ++w) {
        addWordAssign(nodep, w, new AstNodeExpr(1, 1));
    }
    return true;
}
