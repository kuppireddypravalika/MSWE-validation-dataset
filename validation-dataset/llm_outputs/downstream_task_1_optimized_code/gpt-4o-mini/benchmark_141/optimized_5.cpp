#include "payload.h"

bool equals(SetupPayload &self, SetupPayload &input) {
    // Early exit for comparing fundamental types
    if (self.version != input.version || self.vendorID != input.vendorID ||
        self.productID != input.productID || self.requiresCustomFlow != input.requiresCustomFlow ||
        self.rendezvousInformation != input.rendezvousInformation || self.discriminator != input.discriminator ||
        self.setUpPINCode != input.setUpPINCode) {
        return false;
    }

    // Optimize vendor data comparison
    if (self.optionalVendorData.size() != input.optionalVendorData.size())
        return false;
    for (const auto& [tag, inputInfo] : input.optionalVendorData) {
        OptionalQRCodeInfo info;
        if (self.getOptionalVendorData(tag, info) != 0 ||
            inputInfo.type != info.type ||
            inputInfo.data != info.data ||
            inputInfo.int32 != info.int32) {
            return false;
        }
    }

    // Optimize extension data comparison
    if (self.optionalExtensionData.size() != input.optionalExtensionData.size())
        return false;
    for (const auto& [tag, inputInfo] : input.optionalExtensionData) {
        OptionalQRCodeInfoExtension info;
        if (self.getOptionalExtensionData(tag, info) != 0 ||
            inputInfo.type != info.type ||
            inputInfo.data != info.data ||
            inputInfo.int32 != info.int32 ||
            inputInfo.int64 != info.int64 ||
            inputInfo.uint32 != info.uint32 ||
            inputInfo.uint64 != info.uint64) {
            return false;
        }
    }
    return true;
}

// Explicit template instantiation statements
// (Any explicit template instantiation statements should go here unchanged as per requirement)