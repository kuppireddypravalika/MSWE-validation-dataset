#include <chrono>
#include <iostream>
#include <string>

float train_model(int epochs, int dataset_size, int feature_dim);

int main(int argc, char* argv[]) {
    std::string mode = "perf";
    int epochs = 100;
    int dataset_size = 80000;
    int feature_dim = 128;
    int repeat = 1;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--mode=correct") mode = "correct";
        else if (arg == "--mode=perf") mode = "perf";
        else if (arg.rfind("--epochs=",0) == 0) epochs = std::stoi(arg.substr(9));
        else if (arg.rfind("--size=",0) == 0) dataset_size = std::stoi(arg.substr(7));
        else if (arg.rfind("--dim=",0) == 0) feature_dim = std::stoi(arg.substr(6));
        else if (arg.rfind("--repeat=",0) == 0) repeat = std::stoi(arg.substr(9));
    }

    if (mode == "correct") {
        float sum = train_model(epochs, dataset_size, feature_dim);
        std::cout << sum << '\n';
    } else {
        using clock = std::chrono::high_resolution_clock;
        auto t1 = clock::now();
        for (int i = 0; i < repeat; ++i)
            train_model(epochs, dataset_size, feature_dim);
        auto t2 = clock::now();
        std::cout << "Time: "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()
                  << " ms\n";
    }
    return 0;
}
