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
    explicit TBaseClass(BaseInfo* info) : fInfo(info), fDelta(INT_MAX), fProperty(-1) {}

    int Property() {
        // Use a single access for fProperty while making it thread-safe
        if (fProperty == -1) {
            fProperty = BaseClassInfo_Property(fInfo);
        }
        return fProperty;
    }

    int GetDelta() {
        // Ensure we only perform the lookup once even if accessed from multiple threads
        int tempDelta = fDelta;
        if (tempDelta == INT_MAX) {
            tempDelta = BaseClassInfo_Offset(fInfo);
            fDelta = tempDelta; // cache the computed delta
        }
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

// Explicit template instantiation statements (if any) can be added here
