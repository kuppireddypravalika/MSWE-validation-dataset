#include "common.hpp"

void consume(Vehicle* m_vehicle, const ServoPosition* msg) {
    if (UUV* v = dynamic_cast<UUV*>(m_vehicle)) {
        if (msg->id < 4) { // Assuming fins size is always 4
            v->fins[msg->id] += msg->value;
        }
    }
}