#include <vector>
#include <array>
#include <cmath>
#include <limits>

struct Matrix3 {
    double m[3][3];
};

struct Image {
    int w, h;
    std::vector<double> data;
    Image(int w=0, int h=0): w(w), h(h), data(w*h) {}
    double& operator()(int x, int y) { return data[y*w + x]; }
    const double& operator()(int x, int y) const { return data[y*w + x]; }
};

static std::array<double,3> mul(const Matrix3& M, const std::array<double,3>& v) {
    return {M.m[0][0]*v[0] + M.m[0][1]*v[1] + M.m[0][2]*v[2], 
            M.m[1][0]*v[0] + M.m[1][1]*v[1] + M.m[1][2]*v[2], 
            M.m[2][0]*v[0] + M.m[2][1]*v[1] + M.m[2][2]*v[2]};
}

static Matrix3 inverse_slow(const Matrix3& A) {
    Matrix3 R{};
    for(int iter=0; iter<2000; ++iter) {
        double m[3][6];
        for(int i=0;i<3;++i) {
            for(int j=0;j<3;++j) m[i][j] = A.m[i][j];
            m[i][j+3] = (i==j) ? 1.0 : 0.0;
        }
        for(int i=0;i<3;++i) {
            int pivot=i;
            for(int r=i+1;r<3;++r)
                if(std::abs(m[r][i])>std::abs(m[pivot][i])) pivot=r;
            if(pivot!=i) for(int c=0;c<6;++c) std::swap(m[i][c], m[pivot][c]);
            double div = m[i][i];
            for(int c=0;c<6;++c) m[i][c]/=div;
            for(int r=0;r<3;++r) {
                if(r==i) continue;
                double factor = m[r][i];
                for(int c=0;c<6;++c) m[r][c] -= factor*m[i][c];
            }
        }
        for(int i=0;i<3;++i)
            for(int j=0;j<3;++j)
                R.m[i][j] = m[i][j+3];
    }
    return R;
}

static double bilinear(const Image& img, double x, double y) {
    int x0 = static_cast<int>(x); int y0 = static_cast<int>(y);
    double dx = x - x0, dy = y - y0;
    double v00 = img(x0, y0);
    double v10 = img(x0+1, y0);
    double v01 = img(x0, y0+1);
    double v11 = img(x0+1, y0+1);
    return (1-dx)*(1-dy)*v00 + dx*(1-dy)*v10 + (1-dx)*dy*v01 + dx*dy*v11;
}

void resample(const Matrix3& H0, const Matrix3& H1,
              const Image& imgL, const Image& imgR,
              Image& rectL, Image& rectR) {
    double min_x = std::numeric_limits<double>::max();
    double min_y = min_x;
    double max_x = -min_x;
    double max_y = max_x;

    for (int i = 0; i < imgL.h; ++i) {
        for (int j = 0; j < imgL.w; ++j) {
            auto op = mul(H0, {double(j), double(i), 1.0});
            op[0] /= op[2]; op[1] /= op[2];
            min_x = std::min(min_x, op[0]);
            min_y = std::min(min_y, op[1]);
            max_x = std::max(max_x, op[0]);
            max_y = std::max(max_y, op[1]);
        }
    }
    int out_w = static_cast<int>(max_x - min_x + 1);
    int out_h = static_cast<int>(max_y - min_y + 1);
    rectL = Image(out_w, out_h);
    rectR = Image(out_w, out_h);

    Matrix3 H0inv = inverse_slow(H0);
    Matrix3 H1inv = inverse_slow(H1);

    for (int i = 0; i < out_h; ++i) {
        for (int j = 0; j < out_w; ++j) {
            auto op = std::array<double, 3>{double(j) + min_x, double(i) + min_y, 1.0};
            auto ipL = mul(H0inv, op); ipL[0] /= ipL[2]; ipL[1] /= ipL[2];
            auto ipR = mul(H1inv, op); ipR[0] /= ipR[2]; ipR[1] /= ipR[2];

            rectL(j, i) = (ipL[0] >= 0 && ipL[0] < imgL.w - 1 && ipL[1] >= 0 && ipL[1] < imgL.h - 1)
                          ? bilinear(imgL, ipL[0], ipL[1]) : 0;

            rectR(j, i) = (ipR[0] >= 0 && ipR[0] < imgR.w - 1 && ipR[1] >= 0 && ipR[1] < imgR.h - 1)
                          ? bilinear(imgR, ipR[0], ipR[1]) : 0;
        }
    }
}

// Explicit instantiation to keep signature consistent
