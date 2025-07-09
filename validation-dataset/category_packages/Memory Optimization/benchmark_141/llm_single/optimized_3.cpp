#include "payload.h"


bool equals(SetupPayload &self, SetupPayload &input) {
    // Check basic fields with early exit
    if (self.version != input.version || self.vendorID != input.vendorID ||
        self.productID != input.productID || self.requiresCustomFlow != input.requiresCustomFlow ||
        self.rendezvousInformation != input.rendezvousInformation || self.discriminator != input.discriminator ||
        self.setUpPINCode != input.setUpPINCode)
        return false;

    // Check optional vendor data size
    auto &selfVendorData = self.optionalVendorData;
    auto &inputVendorData = input.optionalVendorData;
    if (selfVendorData.size() != inputVendorData.size())
        return false;

    // Compare optional vendor data
    for (const auto &inputInfo : inputVendorData) {
        OptionalQRCodeInfo info;
        auto err = self.getOptionalVendorData(inputInfo.first, info);
        if (err != 0 || inputInfo.second.type != info.type ||
            inputInfo.second.data != info.data || inputInfo.second.int32 != info.int32)
            return false;
    }

    // Check optional extension data size
    auto &selfExtensionData = self.optionalExtensionData;
    auto &inputExtensionData = input.optionalExtensionData;
    if (selfExtensionData.size() != inputExtensionData.size())
        return false;

    // Compare optional extension data
    for (const auto &inputInfo : inputExtensionData) {
        OptionalQRCodeInfoExtension info;
        auto err = self.getOptionalExtensionData(inputInfo.first, info);
        if (err != 0 || inputInfo.second.type != info.type ||
            inputInfo.second.data != info.data || inputInfo.second.int32 != info.int32 ||
            inputInfo.second.int64 != info.int64 ||
            inputInfo.second.uint32 != info.uint32 ||
            inputInfo.second.uint64 != info.uint64)
            return false;
    }
    return true;
}


// Explicit template instantiation statements

