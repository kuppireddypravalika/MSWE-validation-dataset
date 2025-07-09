#include <vector>
#include <string>
#include <cmath>

struct Kernel { bool fastMath; };

static Kernel build_kernel_from_file(const std::string&, const std::string&, const std::string& opts) {
    Kernel k;
    k.fastMath = opts.find("-cl-fast-relaxed-math") != std::string::npos;
    return k;
}

static std::vector<Kernel> build_kernels_original() {
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

    std::vector<Kernel> kernels;
    kernels.push_back(build_kernel_from_file("src/opencl/cl/dipole.cl","update_dipole_fields", default_opts));
    kernels.push_back(build_kernel_from_file("src/opencl/cl/dipole.cl","update_atm_dipole_fields", default_opts));
    kernels.push_back(build_kernel_from_file("src/opencl/cl/dipole.cl","update_cell_magnetization", default_opts));
    kernels.push_back(build_kernel_from_file("src/opencl/cl/external_fields.cl","update_external_fields", default_opts));
    kernels.push_back(build_kernel_from_file("src/opencl/cl/csrmatmul.cl","matmul", default_opts));
    kernels.push_back(build_kernel_from_file("src/opencl/cl/spin_fields.cl","update_nexch_spin_fields", default_opts));
    kernels.push_back(build_kernel_from_file("src/opencl/cl/random.cl","gen_grands", default_opts));
    kernels.push_back(build_kernel_from_file("src/opencl/cl/llg_heun.cl","llg_heun_predictor_step", default_opts));
    kernels.push_back(build_kernel_from_file("src/opencl/cl/llg_heun.cl","llg_heun_corrector_step", default_opts));
    return kernels;
}

static void run_kernels_original(const std::vector<Kernel>& kernels, std::vector<float>& data) {
    for (const auto& k : kernels) {
        for (size_t i = 0; i < data.size(); ++i) {
            float x = data[i];
            if (k.fastMath) {
                data[i] = std::fmaf(x, x, 1.0f);
            } else {
                volatile float exp = 2.0f; // prevent constant-folding
                data[i] = std::pow(x, exp) + 1.0f;
            }
        }
    }
}

double process(std::vector<float>& data) {
    auto kernels = build_kernels_original();
    run_kernels_original(kernels, data);
    double sum = 0.0;
    for (float v : data) sum += v;
    return sum;
}
