#include <cmath>

struct EvaluationOptions {};

const int STRUCT_ADDITION = 1;
const int STRUCT_FUNCTION = 2;

struct MathStructure {
    double value;
    bool has_addition;
    bool has_function;
    MathStructure(double v=0, bool add=false, bool func=false)
        : value(v), has_addition(add), has_function(func) {}
    void replace(const MathStructure&, const MathStructure&) {}
    void eval(const EvaluationOptions& eo = EvaluationOptions()) {
        volatile double tmp = value;
        for(int i=0;i<10; ++i) tmp = std::sin(tmp);
    }
    void calculatesub(const EvaluationOptions& eo1 = EvaluationOptions(), const EvaluationOptions& eo2 = EvaluationOptions(), bool=false) {
        volatile double tmp = value;
        for(int i=0;i<500; ++i) tmp = std::sin(tmp);
    }
    bool isNumber() const { return true; }
    MathStructure number() const { return *this; }
    int getBoolean() const { return value != 0; }
    bool containsType(int t) const { return (t == STRUCT_ADDITION && has_addition) || (t == STRUCT_FUNCTION && has_function); }
};

int for_function(int start, int end) {
    EvaluationOptions eo;
    MathStructure mstruct(0);
    MathStructure mcounter(start);
    while (mcounter.value < end) {
        mstruct.value = mcounter.value;
        mstruct.eval(eo);
        mstruct.calculatesub(eo, eo, false);
        ++mcounter.value;
        mcounter.calculatesub(eo, eo, false);
    }
    return static_cast<int>(mstruct.value);
}

// explicit instantiation
int (*dummy_orig)(int,int) = for_function;