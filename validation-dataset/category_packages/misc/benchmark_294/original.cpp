#include "common.hpp"

void consume(Vehicle* m_vehicle, const ServoPosition* msg) {
    UUV* v = dynamic_cast<UUV*>(m_vehicle);
    v->updateFin(msg->id, msg->value);
}
