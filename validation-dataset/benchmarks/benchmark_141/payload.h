#pragma once
#include <map>
#include <vector>
#include <string>
#include <cstdint>

enum OptionalQRCodeInfoType {
    optionalQRCodeInfoTypeUnknown,
    optionalQRCodeInfoTypeString,
    optionalQRCodeInfoTypeInt32,
    optionalQRCodeInfoTypeUInt32
};

struct OptionalQRCodeInfo {
    uint8_t tag = 0;
    OptionalQRCodeInfoType type = optionalQRCodeInfoTypeUnknown;
    std::string data;
    int32_t int32 = 0;
};

struct OptionalQRCodeInfoExtension {
    uint8_t tag = 0;
    OptionalQRCodeInfoType type = optionalQRCodeInfoTypeUnknown;
    std::string data;
    int32_t int32 = 0;
    int64_t int64 = 0;
    uint32_t uint32 = 0;
    uint64_t uint64 = 0;
};

class SetupPayload {
public:
    uint8_t version = 0;
    uint16_t vendorID = 0;
    uint16_t productID = 0;
    bool requiresCustomFlow = false;
    uint8_t rendezvousInformation = 0;
    uint16_t discriminator = 0;
    uint32_t setUpPINCode = 0;

    std::map<uint8_t, OptionalQRCodeInfo> optionalVendorData;
    std::map<uint8_t, OptionalQRCodeInfoExtension> optionalExtensionData;

    std::vector<OptionalQRCodeInfo> getAllOptionalVendorData() const {
        std::vector<OptionalQRCodeInfo> out;
        for (const auto &e : optionalVendorData) out.push_back(e.second);
        return out;
    }

    std::vector<OptionalQRCodeInfoExtension> getAllOptionalExtensionData() const {
        std::vector<OptionalQRCodeInfoExtension> out;
        for (const auto &e : optionalExtensionData) out.push_back(e.second);
        return out;
    }

    int getOptionalVendorData(uint8_t tag, OptionalQRCodeInfo &info) const {
        auto it = optionalVendorData.find(tag);
        if (it == optionalVendorData.end()) return 1; // error
        info = it->second;
        return 0;
    }

    int getOptionalExtensionData(uint8_t tag, OptionalQRCodeInfoExtension &info) const {
        auto it = optionalExtensionData.find(tag);
        if (it == optionalExtensionData.end()) return 1;
        info = it->second;
        return 0;
    }
};

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

bool equals(SetupPayload &a, SetupPayload &b);

