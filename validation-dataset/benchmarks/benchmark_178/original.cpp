#include <cmath>
#include <vector>

enum ExprType { EN_AUTO, EN_OTHER };

struct Expression {
    ExprType type;
    Expression* left;
    Expression* right;
};

struct ListNode {
    Expression* data;
    ListNode* next;
};

static const int LOOP_COUNT = 270000;

static void heavy_compute() {
    volatile double sum = 0.0;
    for (int i = 0; i < LOOP_COUNT; ++i)
        sum += std::sqrt(i + 1.0);
}

static void gen_expr(Expression* node) {
    heavy_compute();
    (void)node;
}

static void gen_void(Expression* node) {
    gen_expr(node);
}

void process_list(ListNode* head) {
    for (ListNode* n = head; n; n = n->next) {
        gen_void(n->data);
    }
}
