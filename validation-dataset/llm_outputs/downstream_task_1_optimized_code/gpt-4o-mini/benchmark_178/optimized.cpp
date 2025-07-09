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

void process_list(ListNode* head) {
    ListNode* current = head;
    while (current) {
        gen_expr(current->data);
        current = current->next;
    }
}

static void gen_void(Expression* node) {
    gen_expr(node);
}
