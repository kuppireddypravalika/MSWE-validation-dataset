#include <vector>
#include <string>
#include <cmath>
#include <numeric>
#include <iostream>

struct Kernel { bool fastMath; };

static Kernel build_kernel_from_file(const std::string&, const std::string&, const std::string& opts) {
    return { opts.find("-cl-fast-relaxed-math") != std::string::npos };
}

static std::vector<Kernel> build_kernels_optimized() {
    std::string default_opts("-Isrc/opencl/cl ");
#ifdef OPENCL_DP
    default_opts += "-DOPENCL_DP ";
#endif
#ifdef OPENCL_USE_NATIVE_FUNCTIONS
    default_opts += "-DOPENCL_USE_NATIVE_FUNCTIONS ";
#endif
#ifdef USE_VECTOR_TYPE
    default_opts += "-DUSE_VECTOR_TYPE ";
#endif

    return { 
        build_kernel_from_file("src/opencl/cl/dipole.cl","update_dipole_fields", default_opts), 
        build_kernel_from_file("src/opencl/cl/dipole.cl","update_atm_dipole_fields", default_opts),
        build_kernel_from_file("src/opencl/cl/dipole.cl","update_cell_magnetization", default_opts),
        build_kernel_from_file("src/opencl/cl/external_fields.cl","update_external_fields", default_opts),
        build_kernel_from_file("src/opencl/cl/csrmatmul.cl","matmul", default_opts),
        build_kernel_from_file("src/opencl/cl/spin_fields.cl","update_nexch_spin_fields", default_opts),
        build_kernel_from_file("src/opencl/cl/random.cl","gen_grands", default_opts),
        build_kernel_from_file("src/opencl/cl/llg_heun.cl","llg_heun_predictor_step", default_opts),
        build_kernel_from_file("src/opencl/cl/llg_heun.cl","llg_heun_corrector_step", default_opts)
    };
}

static void run_kernels_optimized(const std::vector<Kernel>& kernels, std::vector<float>& data) {
    const size_t dataSize = data.size();
    for (const auto& k : kernels) {
        if (k.fastMath) {
            for (size_t i = 0; i < dataSize; ++i) {
                data[i] = std::fmaf(data[i], data[i], 1.0f);
            }
        } else {
            for (size_t i = 0; i < dataSize; ++i) {
                volatile float exp = 2.0f; // prevent constant-folding
                data[i] = std::pow(data[i], exp) + 1.0f;
            }
        }
    }
}

double process(std::vector<float>& data) {
    auto kernels = build_kernels_optimized();
    run_kernels_optimized(kernels, data);
    return std::accumulate(data.begin(), data.end(), 0.0);
}

// Original instantiation statements

