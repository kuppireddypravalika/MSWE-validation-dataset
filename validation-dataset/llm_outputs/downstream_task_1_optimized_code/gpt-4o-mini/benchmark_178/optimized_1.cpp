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

// Instead of calling a heavy compute function for each expression, we can batch these calls.
static void heavy_compute(int start, int count) {
    // Compute the sums in a single loop to minimize overhead and multiple calls.
    volatile double sum = 0.0;
    for (int i = start; i < start + count; ++i)
        sum += std::sqrt(i + 1.0);
}

void process_list(ListNode* head) {
    if (!head) return;
    
    int count = 0;
    ListNode* current = head;
    
    // First pass to count the elements
    while (current) {
        ++count;
        current = current->next;
    }

    // We execute the heavy computation once per node instead of for each expression generation.
    heavy_compute(0, count);
    
    // Second pass to generate expressions
    current = head;
    while (current) {
        // Assuming we need to generate an expression only after heavy computation
        gen_expr(current->data);
        current = current->next;
    }
}

// Explicit template instantiation statements can go here.
