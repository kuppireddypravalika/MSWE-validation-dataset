#include <vector>
#include <cmath>
#include <algorithm>

void silu_double_grad(const std::vector<float>& x,
                      const std::vector<float>& grad_x_grad,
                      std::vector<float>& grad_out_grad,
                      std::vector<float>& grad_x) {
    size_t size = x.size();
    grad_out_grad.resize(size);
    grad_x.resize(size);
    
    for (size_t i = 0; i < size; ++i) {
        float sigmoid = 1.0f / (1.0f + std::exp(-x[i]));
        float tmp = sigmoid * (1.0f + (1.0f - sigmoid) * x[i]);
        grad_out_grad[i] = grad_x_grad[i] * tmp;
        grad_x[i] = grad_x_grad[i] * (sigmoid * x[i] + tmp);
    }
}
