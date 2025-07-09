#include "common.hpp"

void consume(Vehicle* m_vehicle, const ServoPosition* msg) {
    // Use a static_cast instead of dynamic_cast for performance, assuming m_vehicle is always of type UUV
    UUV* v = static_cast<UUV*>(m_vehicle);
    // Directly update the fin to avoid potential bounds checking in the method
    if (msg->id < v->fins.size()) {
        v->fins[msg->id] += msg->value;
    }
}
