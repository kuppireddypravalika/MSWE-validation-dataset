#include "vtkMath_after.h"

long long run(unsigned long long loops)
{
    volatile long long sum = 0;
    double value = 0.0;
    for (unsigned long long i = 0; i < loops; ++i) {
        sum += vtkMath::Floor(value);
        value += 0.1;
    }
    return sum;
}
