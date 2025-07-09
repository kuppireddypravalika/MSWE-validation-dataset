#include <vector>
#include <random>
#include <cmath>
#include <numeric>

// Match the original code: Adam optimizer with batch size 6
constexpr int64_t kTrainBatchSize = 6;

// Simple Adam optimizer implementation
class Adam {
public:
    Adam(std::vector<float>& params, float lr)
        : params_(params), lr_(lr), m_(params.size(), 0.0f), v_(params.size(), 0.0f), t_(0) {}

    void step(const std::vector<float>& grad) {
        ++t_;
        const float beta1 = 0.9f;
        const float beta2 = 0.999f;
        const float eps = 1e-8f;
        for (size_t i = 0; i < params_.size(); ++i) {
            m_[i] = beta1 * m_[i] + (1 - beta1) * grad[i];
            v_[i] = beta2 * v_[i] + (1 - beta2) * grad[i] * grad[i];
            float mhat = m_[i] / (1 - std::pow(beta1, t_));
            float vhat = v_[i] / (1 - std::pow(beta2, t_));
            params_[i] -= lr_ * mhat / (std::sqrt(vhat) + eps);
        }
    }
private:
    std::vector<float>& params_;
    float lr_;
    std::vector<float> m_, v_;
    int t_;
};

// Generate random dataset and train for a number of epochs
float train_model(int epochs, int dataset_size, int feature_dim) {
    std::mt19937 gen(0);
    std::uniform_real_distribution<float> dis(-1.0f, 1.0f);

    // Create dataset
    std::vector<std::vector<float>> data(dataset_size, std::vector<float>(feature_dim));
    std::vector<int> labels(dataset_size);
    for (int i = 0; i < dataset_size; ++i) {
        for (int j = 0; j < feature_dim; ++j) data[i][j] = dis(gen);
        labels[i] = i % 2;
    }

    // Parameters to learn
    std::vector<float> weights(feature_dim, 0.1f);
    Adam opt(weights, 0.001f);

    std::vector<float> grad(feature_dim);
    for (int epoch = 0; epoch < epochs; ++epoch) {
        for (int i = 0; i < dataset_size; i += kTrainBatchSize) {
            int bs = std::min(kTrainBatchSize, dataset_size - i);
            std::fill(grad.begin(), grad.end(), 0.0f);
            for (int b = 0; b < bs; ++b) {
                float dot = std::inner_product(weights.begin(), weights.end(), data[i + b].begin(), 0.0f);
                float y = 1.0f / (1.0f + std::exp(-dot));
                float diff = y - labels[i + b];
                for (int k = 0; k < feature_dim; ++k) grad[k] += diff * data[i + b][k];
            }
            for (int k = 0; k < feature_dim; ++k) grad[k] /= bs;
            opt.step(grad);
        }
    }
    return std::accumulate(weights.begin(), weights.end(), 0.0f);
}

// Explicit template instantiation statements go here (as they are in the provided code)
