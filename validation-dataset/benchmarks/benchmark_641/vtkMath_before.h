#pragma once
#include <cmath>

class vtkMath {
public:
    static int Floor(double x) {
        return static_cast<int>(std::floor(x));
    }
};
