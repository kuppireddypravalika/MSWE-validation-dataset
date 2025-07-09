#include <chrono>
#include <iostream>
#include <string>
#include <vector>

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

std::vector<std::string> get_camera_device_list(CameraImpl& impl);

int main(int argc, char* argv[]) {
    std::string mode = "perf";
    size_t devices = 2000;
    int repeat = 450000;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--mode=correct")
            mode = "correct";
        else if (arg == "--mode=perf")
            mode = "perf";
        else if (arg.rfind("--devices=", 0) == 0)
            devices = std::stoul(arg.substr(10));
        else if (arg.rfind("--repeat=", 0) == 0)
            repeat = std::stoi(arg.substr(9));
    }

    CameraImpl cam(devices);

    if (mode == "correct") {
        auto result = get_camera_device_list(cam);
        for (const auto& s : result)
            std::cout << s << ' ';
        std::cout << '\n';
    } else {
        using clock = std::chrono::high_resolution_clock;
        auto t1 = clock::now();
        for (int i = 0; i < repeat; ++i)
            get_camera_device_list(cam);
        auto t2 = clock::now();

        std::cout << "Time: "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()
                  << " ms\n";
    }

    return 0;
}
