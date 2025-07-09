#include "common.hpp"

void consume(Vehicle* m_vehicle, const ServoPosition* msg) {
    // Use a static_cast instead of dynamic_cast for performance, since UUV is expected type.
    UUV* v = static_cast<UUV*>(m_vehicle);
    v->updateFin(msg->id, msg->value);
}

// Explicit template instantiation statements can go here, example:
// template class SomeTemplateClass<SomeType>;
