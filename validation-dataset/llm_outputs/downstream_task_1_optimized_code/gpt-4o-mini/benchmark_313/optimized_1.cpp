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
    return { M.m[0][0]*v[0] + M.m[0][1]*v[1] + M.m[0][2]*v[2],
             M.m[1][0]*v[0] + M.m[1][1]*v[1] + M.m[1][2]*v[2],
             M.m[2][0]*v[0] + M.m[2][1]*v[1] + M.m[2][2]*v[2] };
}

static Matrix3 inverse_fast(const Matrix3& A) {
    // Using a pre-calculated determinant for faster inversion
    double det = A.m[0][0] * (A.m[1][1] * A.m[2][2] - A.m[1][2] * A.m[2][1])
                 - A.m[0][1] * (A.m[1][0] * A.m[2][2] - A.m[1][2] * A.m[2][0])
                 + A.m[0][2] * (A.m[1][0] * A.m[2][1] - A.m[1][1] * A.m[2][0]);
    double inv_det = 1.0 / det;
    Matrix3 inv;
    inv.m[0][0] = inv_det * (A.m[1][1] * A.m[2][2] - A.m[1][2] * A.m[2][1]);
    inv.m[0][1] = inv_det * (A.m[0][2] * A.m[2][1] - A.m[0][1] * A.m[2][2]);
    inv.m[0][2] = inv_det * (A.m[0][1] * A.m[1][2] - A.m[0][2] * A.m[1][1]);
    inv.m[1][0] = inv_det * (A.m[1][2] * A.m[2][0] - A.m[1][0] * A.m[2][2]);
    inv.m[1][1] = inv_det * (A.m[0][0] * A.m[2][2] - A.m[0][2] * A.m[2][0]);
    inv.m[1][2] = inv_det * (A.m[1][0] * A.m[0][2] - A.m[0][0] * A.m[1][2]);
    inv.m[2][0] = inv_det * (A.m[1][0] * A.m[2][1] - A.m[1][1] * A.m[2][0]);
    inv.m[2][1] = inv_det * (A.m[1][1] * A.m[0][0] - A.m[0][1] * A.m[1][0]);
    inv.m[2][2] = inv_det * (A.m[0][1] * A.m[1][0] - A.m[0][0] * A.m[1][1]);
    return inv;
}

static double bilinear(const Image& img, double x, double y) {
    int x0 = (int)x; int y0 = (int)y;
    double dx = x - x0, dy = y - y0;
    double v00 = img(x0, y0);
    double v10 = img(x0 + 1, y0);
    double v01 = img(x0, y0 + 1);
    double v11 = img(x0 + 1, y0 + 1);
    return (1 - dx) * (1 - dy) * v00 + dx * (1 - dy) * v10 + (1 - dx) * dy * v01 + dx * dy * v11;
}

void resample(const Matrix3& H0, const Matrix3& H1,
              const Image& imgL, const Image& imgR,
              Image& rectL, Image& rectR) {
    double min_x = std::numeric_limits<double>::max();
    double min_y = std::numeric_limits<double>::max();
    double max_x = -std::numeric_limits<double>::max();
    double max_y = -std::numeric_limits<double>::max();
    const int imgL_w = imgL.w;
    const int imgL_h = imgL.h;
    for (int i = 0; i < imgL_h; ++i) {
        for (int j = 0; j < imgL_w; ++j) {
            auto op = mul(H0, { static_cast<double>(j), static_cast<double>(i), 1.0 });
            op[0] /= op[2]; op[1] /= op[2];
            min_x = std::min(min_x, op[0]);
            min_y = std::min(min_y, op[1]);
            max_x = std::max(max_x, op[0]);
            max_y = std::max(max_y, op[1]);
        }
    }
    int out_w = static_cast<int>(max_x - min_x + 1); // Using static_cast for clearer intent
    int out_h = static_cast<int>(max_y - min_y + 1);
    rectL = Image(out_w, out_h);
    rectR = Image(out_w, out_h);

    Matrix3 H0inv = inverse_fast(H0);
    Matrix3 H1inv = inverse_fast(H1);

    for (int i = 0; i < out_h; ++i) {
        for (int j = 0; j < out_w; ++j) {
            auto op = std::array<double, 3>{ static_cast<double>(j) + min_x, static_cast<double>(i) + min_y, 1.0 };
            auto ipL = mul(H0inv, op); ipL[0] /= ipL[2]; ipL[1] /= ipL[2];
            auto ipR = mul(H1inv, op); ipR[0] /= ipR[2]; ipR[1] /= ipR[2];
            rectL(j, i) = (ipL[0] >= 0 && ipL[0] < imgL.w - 1 && ipL[1] >= 0 && ipL[1] < imgL.h - 1) ? bilinear(imgL, ipL[0], ipL[1]) : 0;
            rectR(j, i) = (ipR[0] >= 0 && ipR[0] < imgR.w - 1 && ipR[1] >= 0 && ipR[1] < imgR.h - 1) ? bilinear(imgR, ipR[0], ipR[1]) : 0;
        }
    }
}

// Explicit instantiation to keep signature consistent