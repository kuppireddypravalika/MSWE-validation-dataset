#include "common.hpp"

void consume(Vehicle* m_vehicle, const ServoPosition* msg) {
    // Use static_cast instead of dynamic_cast for performance reasons
    UUV* v = static_cast<UUV*>(m_vehicle);
    if (msg->id < 4) { // Check the id bounds to prevent out-of-bounds access
        v->updateFin(msg->id, msg->value);
    }
}
