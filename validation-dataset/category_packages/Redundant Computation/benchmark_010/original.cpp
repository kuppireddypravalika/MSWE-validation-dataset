#include <vector>
#include <cmath>

void silu_double_grad(const std::vector<float>& x,
                      const std::vector<float>& grad_x_grad,
                      std::vector<float>& grad_out_grad,
                      std::vector<float>& grad_x) {
    size_t size = x.size();
    grad_out_grad.resize(size);
    grad_x.resize(size);
    for (size_t i = 0; i < size; ++i) {
        float sigmoid = 1.0f / (1.0f + std::exp(-x[i]));
        float tmp1 = 1.0f - sigmoid;
        float tmp2 = 1.0f + tmp1 * x[i];
        grad_out_grad[i] = grad_x_grad[i] * sigmoid * tmp2;
        grad_x[i] = sigmoid * grad_x_grad[i] * x[i] + grad_x_grad[i] * sigmoid * tmp2;
    }
}

