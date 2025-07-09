#ifndef RECYCLING_HPP
#define RECYCLING_HPP
#include <vector>

struct Residence {
    int waste = 100;
    void pullWaste(int amount) { waste -= amount; }
};

struct Recycling {
    bool operational_ = true;
    int capacity = 30;
    int support = 10;

    bool operational() const { return operational_; }
    int wasteProcessingCapacity() const { return capacity; }
    int residentialSupportCount() const { return support; }
};

#endif // RECYCLING_HPP
