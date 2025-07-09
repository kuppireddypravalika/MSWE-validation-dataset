#include "common.hpp"

void consume(Vehicle* m_vehicle, const ServoPosition* msg) {
    UUV* v = static_cast<UUV*>(m_vehicle);
    v->updateFin(msg->id, msg->value);
}
