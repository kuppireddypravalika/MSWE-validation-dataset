#include <climits>
#include <chrono>
#include <thread>

struct BaseInfo {
    int property;
};

const int kIsVirtualBase = 1;

static int BaseClassInfo_Property(BaseInfo* info) {
    return info->property;
}

static int BaseClassInfo_Offset(BaseInfo*) {
    // Simulate expensive lookup that ultimately fails
    std::this_thread::sleep_for(std::chrono::microseconds(200));
    return -1;
}

class TBaseClass {
public:
    explicit TBaseClass(BaseInfo* info)
        : fInfo(info), fDelta(-1), fProperty(-1) {}

    int Property() {
        if (fProperty == -1)
            fProperty = BaseClassInfo_Property(fInfo);
        return fProperty;
    }

    int GetDelta() {
        if (fDelta == -1)
            fDelta = BaseClassInfo_Offset(fInfo);
        return fDelta;
    }

private:
    BaseInfo* fInfo;
    int fDelta;
    int fProperty;
};

int get_delta(BaseInfo* info) {
    static thread_local TBaseClass bc(info);
    return bc.GetDelta();
}

// Explicit template instantiation statements (if any) should remain unchanged.
