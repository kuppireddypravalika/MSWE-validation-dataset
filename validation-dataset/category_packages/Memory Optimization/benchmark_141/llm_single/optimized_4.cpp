#include "payload.h"

bool equals(SetupPayload &self, SetupPayload &input) {
    if (self.version != input.version || self.vendorID != input.vendorID ||
        self.productID != input.productID || self.requiresCustomFlow != input.requiresCustomFlow ||
        self.rendezvousInformation != input.rendezvousInformation || self.discriminator != input.discriminator ||
        self.setUpPINCode != input.setUpPINCode)
        return false;

    auto inputOptionalVendorData = input.getAllOptionalVendorData();
    if (self.optionalVendorData.size() != inputOptionalVendorData.size())
        return false;
    for (const auto &inputInfo : inputOptionalVendorData) {
        OptionalQRCodeInfo info;
        if (self.getOptionalVendorData(inputInfo.tag, info) != 0) return false;
        if (inputInfo.type != info.type || inputInfo.data != info.data || inputInfo.int32 != info.int32)
            return false;
    }

    auto inputOptionalExtensionData = input.getAllOptionalExtensionData();
    if (self.optionalExtensionData.size() != inputOptionalExtensionData.size())
        return false;
    for (const auto &inputInfo : inputOptionalExtensionData) {
        OptionalQRCodeInfoExtension info;
        if (self.getOptionalExtensionData(inputInfo.tag, info) != 0) return false;
        if (inputInfo.type != info.type || inputInfo.data != info.data || inputInfo.int32 != info.int32 || 
            inputInfo.int64 != info.int64 || inputInfo.uint32 != info.uint32 || inputInfo.uint64 != info.uint64)
            return false;
    }
    return true;
}
