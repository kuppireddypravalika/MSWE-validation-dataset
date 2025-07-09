#include <chrono>
#include <iostream>
#include <string>

size_t parse_source(const std::string& src);

std::string generate_source(int functions, int bodyLines)
{
    std::string src;
    for (int f = 0; f < functions; ++f) {
        src += "void func" + std::to_string(f) + "() {\n";
        for (int i = 0; i < bodyLines; ++i) {
            src += "    int a" + std::to_string(i) + " = " + std::to_string(i) + ";\n";
        }
        src += "}\n";
    }
    return src;
}

int main(int argc, char* argv[])
{
    std::string mode = "perf";
    int functions = 150;
    int body = 300;
    int repeat = 3000;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--mode=correct") mode = "correct";
        else if (arg == "--mode=perf") mode = "perf";
        else if (arg.rfind("--functions=", 0) == 0) functions = std::stoi(arg.substr(12));
        else if (arg.rfind("--body=", 0) == 0) body = std::stoi(arg.substr(7));
        else if (arg.rfind("--repeat=", 0) == 0) repeat = std::stoi(arg.substr(9));
    }

    std::string src = generate_source(functions, body);

    if (mode == "correct") {
        size_t val = parse_source(src);
        std::cout << val << std::endl;
    } else {
        using clock = std::chrono::high_resolution_clock;
        auto t1 = clock::now();
        for (int r = 0; r < repeat; ++r)
            parse_source(src);
        auto t2 = clock::now();

        std::cout << "Time: "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()
                  << " ms\n";
    }
    return 0;
}
