#include <vector>
#include <cstring>

class CodeGenOpenCLDevOriginal {
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
        for (size_t i = 0; i < n; ++i) {
            out[i] = a[i] * b[i] + c[i];
        }
    }
};

extern "C" void compute(const float* a,const float* b,const float* c,float* out,size_t n) {
    static CodeGenOpenCLDevOriginal cg;
    static bool initialized = false;
    if (!initialized) { cg.init_module(); initialized = true; }
    cg.run_kernel(a,b,c,out,n);
}
