#include <cmath>

// Use library floor through a volatile function pointer
static double (*volatile floor_ptr)(double) = std::floor;

long long run(unsigned long long loops)
{
    volatile long long sum = 0;
    double value = 0.0;
    for (unsigned long long i = 0; i < loops; ++i) {
        sum += static_cast<int>(floor_ptr(value));
        value += 0.1;
    }
    return sum;
}
