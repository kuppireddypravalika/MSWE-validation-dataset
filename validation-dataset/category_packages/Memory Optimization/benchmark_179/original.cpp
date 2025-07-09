#include <vector>
#include <string>

struct DeviceMonitor {
    std::vector<std::string> devices;
    explicit DeviceMonitor(size_t n) {
        devices.reserve(n);
        for (size_t i = 0; i < n; ++i)
            devices.emplace_back("device" + std::to_string(i));
    }
    std::vector<std::string> enumerate() const {
        std::vector<std::string> out;
        out.reserve(devices.size());
        for (const auto& d : devices)
            out.push_back(d);
        return out;
    }
};

struct CameraImpl {
    DeviceMonitor monitor;
    std::vector<std::string> m_devices;
    explicit CameraImpl(size_t n) : monitor(n), m_devices(monitor.enumerate()) {}
};

std::vector<std::string> get_camera_device_list(CameraImpl& impl) {
    std::vector<std::string> dlist;
    auto devs = impl.monitor.enumerate();
    for (const auto& d : devs)
        dlist.push_back(d);
    return dlist;
}
