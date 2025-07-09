#include <vector>
#include <random>
#include <cmath>
#include <numeric>

// Optimized: use SGD with momentum and larger batch size
constexpr int64_t kTrainBatchSize = 8;

class SGD {
public:
    SGD(std::vector<float>& params, float lr, float momentum, float weight_decay)
        : params_(params), lr_(lr), momentum_(momentum), wd_(weight_decay), buf_(params.size(), 0.0f) {}

    void step(const std::vector<float>& grad) {
        for (size_t i = 0; i < params_.size(); ++i) {
            float g = grad[i] + wd_ * params_[i];
            buf_[i] = momentum_ * buf_[i] + g;
            params_[i] -= lr_ * buf_[i];
        }
    }
private:
    std::vector<float>& params_;
    float lr_;
    float momentum_;
    float wd_;
    std::vector<float> buf_;
};

float train_model(int epochs, int dataset_size, int feature_dim) {
    std::mt19937 gen(0);
    std::uniform_real_distribution<float> dis(-1.0f, 1.0f);

    std::vector<std::vector<float>> data(dataset_size, std::vector<float>(feature_dim));
    std::vector<int> labels(dataset_size);
    for (int i = 0; i < dataset_size; ++i) {
        for (int j = 0; j < feature_dim; ++j) data[i][j] = dis(gen);
        labels[i] = i % 2;
    }

    std::vector<float> weights(feature_dim, 0.1f);
    SGD opt(weights, 0.01f, 0.9f, 0.0f);

    std::vector<float> grad(feature_dim);
    for (int epoch = 0; epoch < epochs; ++epoch) {
        for (int i = 0; i < dataset_size; i += kTrainBatchSize) {
            int bs = std::min<int>(kTrainBatchSize, dataset_size - i);
            std::fill(grad.begin(), grad.end(), 0.0f);
            for (int b = 0; b < bs; ++b) {
                float dot = 0.0f;
                for (int k = 0; k < feature_dim; ++k) dot += weights[k] * data[i + b][k];
                float y = 1.0f / (1.0f + std::exp(-dot));
                float diff = 0.0f;
                for (int k = 0; k < feature_dim; ++k) grad[k] += diff * data[i + b][k];
            }
            for (int k = 0; k < feature_dim; ++k) grad[k] /= bs;
            opt.step(grad);
        }
    }
    return std::accumulate(weights.begin(), weights.end(), 0.0f);
}
