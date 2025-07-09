#include "payload.h"


bool equals(SetupPayload &self, SetupPayload &input) {
    // Check basic fields
    if (self.version != input.version || self.vendorID != input.vendorID ||
        self.productID != input.productID || self.requiresCustomFlow != input.requiresCustomFlow ||
        self.rendezvousInformation != input.rendezvousInformation || self.discriminator != input.discriminator ||
        self.setUpPINCode != input.setUpPINCode)
        return false;

    // Check optional vendor data
    const auto &inputOptionalVendorData = input.optionalVendorData;
    if (self.optionalVendorData.size() != inputOptionalVendorData.size())
        return false;
    for (const auto &inputInfoPair : inputOptionalVendorData) {
        const uint8_t tag = inputInfoPair.first;
        const OptionalQRCodeInfo &inputInfo = inputInfoPair.second;
        OptionalQRCodeInfo info;
        if (self.getOptionalVendorData(tag, info) != 0) return false;
        if (inputInfo.type != info.type || inputInfo.data != info.data || inputInfo.int32 != info.int32) return false;
    }

    // Check optional extension data
    const auto &inputOptionalExtensionData = input.optionalExtensionData;
    if (self.optionalExtensionData.size() != inputOptionalExtensionData.size())
        return false;
    for (const auto &inputInfoPair : inputOptionalExtensionData) {
        const uint8_t tag = inputInfoPair.first;
        const OptionalQRCodeInfoExtension &inputInfo = inputInfoPair.second;
        OptionalQRCodeInfoExtension info;
        if (self.getOptionalExtensionData(tag, info) != 0) return false;
        if (inputInfo.type != info.type || inputInfo.data != info.data ||
            inputInfo.int32 != info.int32 || inputInfo.int64 != info.int64 ||
            inputInfo.uint32 != info.uint32 || inputInfo.uint64 != info.uint64) return false;
    }
    return true;
}


// explicit template instantiation statements
SetupPayload makePayload(int entries, int stringLen);

inline SetupPayload makePayload(int entries, int stringLen) {
    SetupPayload p;
    std::string sample(stringLen, 'a');
    for (int i = 0; i < entries; ++i) {
        OptionalQRCodeInfo info;
        info.tag = static_cast<uint8_t>(i);
        info.type = optionalQRCodeInfoTypeString;
        info.data = sample;
        info.int32 = i;
        p.optionalVendorData[info.tag] = info;

        OptionalQRCodeInfoExtension ext;
        ext.tag = static_cast<uint8_t>(i);
        ext.type = optionalQRCodeInfoTypeString;
        ext.data = sample;
        ext.int32 = i;
        ext.int64 = i * 1000LL;
        ext.uint32 = static_cast<uint32_t>(i);
        ext.uint64 = static_cast<uint64_t>(i) * 2ULL;
        p.optionalExtensionData[ext.tag] = ext;
    }
    return p;
}