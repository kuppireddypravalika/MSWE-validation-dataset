#include <vector>
#include <cstdint>

constexpr int ONE_PLY = 1;
constexpr int VALUE_ZERO = 0;
constexpr int VALUE_MATED_IN_MAX_PLY = -20000;
constexpr int PawnValueMg = 141;

struct Position {
    bool advanced_pawn_push(int move) const { return move % 7 == 0; }
    int see_sign(int move) const { return (move % 10) - 5; }
};

static int heavy_compute(int move) {
    int x = move;
    for (int i = 0; i < 3300; ++i) {
        x = x * 1664525 + 1013904223;
        x ^= x >> 16;
    }
    return x;
}

int search(const Position& pos, const std::vector<int>& moves, int depth) {
    int bestValue = VALUE_MATED_IN_MAX_PLY + 1;
    int sum = 0;

    for (int move : moves) {
        bool captureOrPromotion = (move & 1);
        bool givesCheck = (move % 4 == 0);
        bool advancedPawn = pos.advanced_pawn_push(move);

        if (bestValue > VALUE_MATED_IN_MAX_PLY && !captureOrPromotion && !givesCheck && !advancedPawn) {
            int predictedDepth = depth - ONE_PLY;
            if (predictedDepth < 8 * ONE_PLY) {
                int see_v = predictedDepth < 4 * ONE_PLY ? VALUE_ZERO : -PawnValueMg * 2 * (predictedDepth - 3 * ONE_PLY) / ONE_PLY;
                if (pos.see_sign(move) < see_v) continue;
            }
        }

        int val = heavy_compute(move);

        if (depth < 3 * ONE_PLY && !givesCheck && bestValue > VALUE_MATED_IN_MAX_PLY && !captureOrPromotion && !advancedPawn && pos.see_sign(move) < VALUE_ZERO) {
            continue;
        }

        sum += val;
    }

    return sum;
}

#ifndef BENCHMARK_LIBRARY
#include <iostream>
int main() {
    Position p;
    std::vector<int> moves(1000);
    for (int i = 0; i < 1000; ++i) moves[i] = i;
    std::cout << search(p, moves, 2) << '\n';
}
#endif
