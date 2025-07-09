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
    // Avoiding the unnecessary sqrt calculation in a loop 
    for (int i = 1; i <= LOOP_COUNT; ++i)
        sum += i * 0.1; // Replace with a simpler calculation for demonstration
}

static void gen_expr(Expression* node) {
    heavy_compute();
    (void)node;
}

void process_list(ListNode* head) {
    for (ListNode* n = head; n != nullptr; n = n->next) {
        gen_expr(n->data);
    }
}

// Explicit template instantiation statements
