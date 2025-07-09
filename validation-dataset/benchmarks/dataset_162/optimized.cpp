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
    int getBoolean() const { return value!=0; }
    bool containsType(int t) const { return (t==STRUCT_ADDITION && has_addition) || (t==STRUCT_FUNCTION && has_function); }
};

int for_function(int start, int end) {
    EvaluationOptions eo;
    MathStructure mstruct(0);
    MathStructure mcounter(start);
    MathStructure mtest;
    MathStructure mcount;
    MathStructure mupdate;
    bool b_eval = true;
    while(true) {
        mtest = MathStructure(mcounter.value < end);
        mtest.eval(eo);
        if(!mtest.getBoolean()) break;
        mupdate = MathStructure(mcounter.value);
        mstruct = mupdate;
        if(b_eval) {
            mstruct.eval(eo);
            if(mstruct.containsType(STRUCT_ADDITION) || mstruct.containsType(STRUCT_FUNCTION))
                b_eval = false;
        } else {
            mstruct.calculatesub(eo, eo, false);
        }
        mcount = MathStructure(mcounter.value + 1);
        mcounter = mcount;
        mcounter.eval(eo);
    }
    return static_cast<int>(mstruct.value);
}

// explicit instantiation
int (*dummy_opt)(int,int) = for_function;
