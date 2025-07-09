#include "p256_helpers.hpp"
#include <cstdlib>

uint64_t p256_multiply(CodeGenerator& cg, const Vec16& A, const Vec16& B, int iters) {
    static const char* disableECCSIMD = std::getenv("TR_disableECCSIMD");
    static const char* disableECCMLGR = std::getenv("TR_disableECCMLGR");
    static const char* disableECCKarat = std::getenv("TR_disableECCKarat");
    static const char* disableVMSL = std::getenv("TR_disableVMSL");

    bool disableSIMDP256 = disableECCSIMD != nullptr;
    bool disableMLGRP256 = disableECCMLGR != nullptr;

    if (!disableVMSL && cg.getS390ProcessorInfo()->supportsArch(1))
        return inlineVMSL256Multiply(A, B, iters);
    if (disableECCKarat == nullptr && cg.getSupportsVectorRegisters())
        return inlineSIMDP256Multiply(A, B, iters);
    if (!disableECCSIMD && cg.getSupportsVectorRegisters())
        return inlineSIMDP256Multiply(A, B, iters);
    else if (!disableMLGRP256)
        return inline256Multiply(A, B, iters);
    return 0;
}
