#include <vector>
#include <string>

struct DeviceMonitor {
    std::vector<std::string> devices;
    explicit DeviceMonitor(size_t n) {
        devices.reserve(n);
        for (size_t i = 0; i < n; ++i)
            devices.emplace_back("device" + std::to_string(i));
    }
    const std::vector<std::string>& enumerate() const {
        return devices;
    }
};

struct CameraImpl {
    DeviceMonitor monitor;
    std::vector<std::string> m_devices;
    explicit CameraImpl(size_t n) : monitor(n), m_devices(monitor.enumerate()) {}
};

std::vector<std::string> get_camera_device_list(CameraImpl& impl) {
    // Directly returning a copy of the monitored devices list instead of allocating unnecessary space
    return impl.monitor.enumerate();
}

// Explicit template instantiation statements, if any
