#include "burned_warehouse.hpp"
#include "agent_optimized.cpp"
#include <iostream>
#include <array>

int main() {
    std::array<unsigned, NUM_JOBS> people = {100, 100, 100, 100, 100}; // Example initialization
    BurnedWarehouse bw(people);
    bw.go_out_phase = 0;

    int result = handle_event(bw);
    std::cout << "Result: " << result << std::endl;
    return 0;
}
