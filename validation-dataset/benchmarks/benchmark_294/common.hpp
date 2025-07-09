#include <vector>
#include <cstdint>

namespace Simulators {
namespace VSIM {

class Vehicle {
public:
    virtual ~Vehicle() {}
};

class UUV : public Vehicle {
public:
    std::vector<float> fins;
    UUV() : fins(4, 0.0f) {}
    void updateFin(uint8_t id, float value) {
        if (id < fins.size())
            fins[id] += value;
    }
};

} // namespace VSIM
} // namespace Simulators

struct ServoPosition {
    uint8_t id;
    float value;
};

using Simulators::VSIM::Vehicle;
using Simulators::VSIM::UUV;
