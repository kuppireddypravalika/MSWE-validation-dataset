#include <cstddef>

struct Position {
    bool non_pawn_material(int /*side*/) const { return true; }
};

struct Stack { int staticEval; };

__attribute__((noinline)) inline int null_search(int depth) {
    volatile int sum = 0;
    for (int i = 0; i < depth * 100000; ++i)
        sum += i;
    return sum;
}

inline int search(Position& pos, Stack* ss, int depth, int eval, int beta) {
    if (depth >= 2 && eval >= beta && pos.non_pawn_material(0)) {
        ss->staticEval = eval;
        null_search(depth);
    }
    return 1; // deterministic result regardless of pruning
}

size_t run(size_t loops) {
    Position pos;
    Stack ss{};
    size_t sum = 0;
    ss.staticEval = -5; // set a default value outside the loop
    for (size_t i = 0; i < loops; ++i) {
        int depth = (static_cast<int>(i % 10) + 1);
        if (i % 5 == 0) {
            ss.staticEval = 5; // set conditionally
        }
        sum += search(pos, &ss, depth, 10, 0);
    }
    return sum;
}

// Explicit template instantiation statements would go here if any.