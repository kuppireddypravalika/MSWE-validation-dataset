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
    for (const OptionalQRCodeInfo &inputInfo : inputOptionalVendorData) {
        OptionalQRCodeInfo info;
        int err = self.getOptionalVendorData(inputInfo.tag, info);
        if (err != 0) return false;
        if (inputInfo.type != info.type) return false;
        if (inputInfo.data != info.data) return false;
        if (inputInfo.int32 != info.int32) return false;
    }

    auto inputOptionalExtensionData = input.getAllOptionalExtensionData();
    if (self.optionalExtensionData.size() != inputOptionalExtensionData.size())
        return false;
    for (const OptionalQRCodeInfoExtension &inputInfo : inputOptionalExtensionData) {
        OptionalQRCodeInfoExtension info;
        int err = self.getOptionalExtensionData(inputInfo.tag, info);
        if (err != 0) return false;
        if (inputInfo.type != info.type) return false;
        if (inputInfo.data != info.data) return false;
        if (inputInfo.int32 != info.int32) return false;
        if (inputInfo.int64 != info.int64) return false;
        if (inputInfo.uint32 != info.uint32) return false;
        if (inputInfo.uint64 != info.uint64) return false;
    }
    return true;
}
