#include <climits>
#include <chrono>
#include <thread>

struct BaseInfo {
    int property;
};

const int kIsVirtualBase = 1;

static inline int BaseClassInfo_Property(BaseInfo* info) {
    return info->property;
}

static inline int BaseClassInfo_Offset(BaseInfo* info) {
    // Simulate expensive lookup that ultimately fails
    std::this_thread::sleep_for(std::chrono::microseconds(200));
    return -1;
}

class TBaseClass {
public:
    explicit TBaseClass(BaseInfo* info)
        : fInfo(info), fDelta(INT_MAX), fProperty(-1) {}

    int Property() {
        if (fProperty == -1)
            fProperty = BaseClassInfo_Property(fInfo);
        return fProperty;
    }

    int GetDelta() {
        if (fDelta == INT_MAX)
            fDelta = BaseClassInfo_Offset(fInfo);
        return fDelta;
    }

private:
    BaseInfo* fInfo;
    int fDelta;
    int fProperty;
};

int get_delta(BaseInfo* info) {
    TBaseClass bc(info);
    return bc.GetDelta();
}

// Explicit template instantiation statements
