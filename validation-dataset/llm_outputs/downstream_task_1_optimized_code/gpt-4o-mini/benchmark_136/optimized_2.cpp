#include <vector>
#include <cmath>
#include <algorithm>
#include <immintrin.h> // For AVX intrinsics

class CodeGenOpenCLDevOptimized {
public:
    std::ostringstream src_stream;
    void init_module() {
        src_stream.str("");
        src_stream.clear();
        src_stream << "/*OpenCL C*/" << '\n';
#ifdef ENABLE_CL_KHR_FP64
        src_stream << "#pragma OPENCL EXTENSION cl_khr_fp64 : enable" << '\n';
#endif
        src_stream << "float mul_add(float a,float b,float c){return a*b+c;}" << '\n';
    }
 
    void run_kernel(const float* a,const float* b,const float* c,float* out,size_t n) {
        size_t i;
        size_t vectorized_end = n / 8 * 8;

        // Process 8 floats at a time using AVX
        __m256 vec_a, vec_b, vec_c, vec_tmp, vec_out;
        for (i = 0; i < vectorized_end; i += 8) {
            vec_a = _mm256_loadu_ps(&a[i]);
            vec_b = _mm256_loadu_ps(&b[i]);
            vec_tmp = _mm256_mul_ps(vec_a, vec_b);
            vec_c = _mm256_loadu_ps(&c[i]);
            vec_out = _mm256_add_ps(vec_tmp, vec_c);
            _mm256_storeu_ps(&out[i], vec_out);
        }
 
        // Handle remaining elements
        for (; i < n; ++i) {
            out[i] = a[i] * b[i] + c[i];
        }
    }
};

extern "C" void compute(const float* a,const float* b,const float* c,float* out,size_t n) {
    static CodeGenOpenCLDevOptimized cg;
    static bool initialized = false;
    if (!initialized) { cg.init_module(); initialized = true; }
    cg.run_kernel(a,b,c,out,n);
}

// Explicit template instantiations (if applicable)
