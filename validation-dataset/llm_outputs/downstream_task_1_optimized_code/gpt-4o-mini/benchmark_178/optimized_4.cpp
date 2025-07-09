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
    double sum = 0.0;
    for (int i = 1; i <= LOOP_COUNT; ++i)
        sum += std::sqrt(i);
}

static void gen_expr(Expression* node) {
    heavy_compute();
    (void)node;
}

void process_list(ListNode* head) {
    if (!head) return;
    ListNode* current = head;
    while (current) {
        gen_expr(current->data);
        current = current->next;
    }
}

// Explicit template instantiation statements can be added here if needed.