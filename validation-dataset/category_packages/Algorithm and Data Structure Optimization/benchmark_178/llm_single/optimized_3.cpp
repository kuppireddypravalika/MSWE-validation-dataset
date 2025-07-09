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

static inline void heavy_compute() {
    volatile double sum = 0.0;
    for (int i = 0; i < LOOP_COUNT; ++i)
        sum += std::sqrt(i + 1.0);
}

static void gen_expr(Expression* node) {
    heavy_compute();
    (void)node;
}

void process_list(ListNode* head) {
    if (!head) return;  // Early exit if head is null
    for (ListNode* n = head; n != nullptr; n = n->next) {
        gen_expr(n->data); // Directly call gen_expr instead of gen_void
    }
}

// Explicit template instantiation statements remain unchanged
