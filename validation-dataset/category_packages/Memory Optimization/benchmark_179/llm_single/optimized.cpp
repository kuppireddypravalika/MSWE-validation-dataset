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
        return devices;  // Directly return the devices vector
    }
};

struct CameraImpl {
    DeviceMonitor monitor;
    std::vector<std::string> m_devices;
    explicit CameraImpl(size_t n) : monitor(n), m_devices(monitor.enumerate()) {}
};

std::vector<std::string> get_camera_device_list(CameraImpl& impl) {
    return impl.monitor.enumerate(); // Directly return the result of the enumerate method
}

// Explicit template instantiation statements remain unchanged below
