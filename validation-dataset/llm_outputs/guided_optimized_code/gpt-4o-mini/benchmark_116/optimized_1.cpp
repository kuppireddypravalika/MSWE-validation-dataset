#include <vector>
#include <memory>
#include <numeric>

struct AstNodeExpr {
    int width;
    std::shared_ptr<std::vector<int>> data;
    AstNodeExpr(int w, int sz) : width(w), data(std::make_shared<std::vector<int>>(sz, w)) {}
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

static AstNodeExpr* cloneAstNodeExpr(AstNodeExpr* expr) {
    AstNodeExpr* clone = new AstNodeExpr(expr->width, expr->data->size());
    clone->data = expr->data; // Share data to avoid deep copy
    return clone;
}

static void appendWordSum(AstNodeAssign* nodep, AstNodeExpr* expr) {
    long long sum = std::accumulate(expr->data->begin(), expr->data->end(), 0LL);
    nodep->assigned.push_back(sum);
    delete expr;
}

static void handleLhsWords(AstNodeAssign* nodep, AstNodeExpr* lhs) {
    for (int w = 0; w < lhs->widthWords(); ++w) {
        appendWordSum(nodep, cloneAstNodeExpr(lhs));
    }
}

static void handleRhsWords(AstNodeAssign* nodep, int widthDifference) {
    for (int w = 0; w < widthDifference; ++w) {
        appendWordSum(nodep, new AstNodeExpr(1, 1));
    }
}

bool expandWide(AstNodeAssign* nodep, AstExtend* rhsp) {
    AstNodeExpr* lhs = nodep->lhsp();
    handleLhsWords(nodep, lhs);
    int widthDifference = nodep->widthWords() - rhsp->widthWords();
    handleRhsWords(nodep, widthDifference);
    return true;
}

// Explicit template instantiation statements (if any) should be included here, unchanged.