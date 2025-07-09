#include <chrono>
#include <iostream>
#include <vector>
#include <string>

struct AstNodeExpr {
    int width;
    std::vector<int> data;
    AstNodeExpr(int w, int sz) : width(w), data(sz, w) {}
    int widthWords() const { return width; }
};
struct AstExtend {
    AstNodeExpr* lhs;
    int width;
    AstExtend(AstNodeExpr* l, int w) : lhs(l), width(w) {}
    AstNodeExpr* lhsp() const { return lhs; }
    int widthWords() const { return width; }
};
struct AstNodeAssign {
    AstNodeExpr* lhs;
    int width;
    std::vector<int> assigned;
    AstNodeAssign(AstNodeExpr* l, int w) : lhs(l), width(w) {}
    AstNodeExpr* lhsp() const { return lhs; }
    int widthWords() const { return width; }
};

bool expandWide(AstNodeAssign* nodep, AstExtend* rhsp);

int main(int argc, char* argv[]) {
    std::string mode = "perf";
    int repeat = 300000;
    int lhs_size = 10000; // size of heavy lhs vector
    int lhs_width = 16;
    int rlhs_width = 8;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--mode=correct") mode = "correct";
        else if (arg == "--mode=perf") mode = "perf";
        else if (arg.rfind("--repeat=", 0) == 0) repeat = std::stoi(arg.substr(9));
        else if (arg.rfind("--lhs_size=",0)==0) lhs_size=std::stoi(arg.substr(11));
    }

    AstNodeExpr lhs(lhs_width, lhs_size);
    AstNodeExpr rlhs(rlhs_width, 10);
    AstExtend ext(&rlhs, lhs_width);
    AstNodeAssign node(&lhs, lhs_width);

    if (mode == "correct") {
        expandWide(&node, &ext);
        std::cout << node.assigned.size() << std::endl;
    } else {
        using clock = std::chrono::high_resolution_clock;
        auto t1 = clock::now();
        for (int i=0;i<repeat;++i) {
            node.assigned.clear();
            expandWide(&node, &ext);
        }
        auto t2 = clock::now();

        std::cout << "Time: "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(t2-t1).count()
                  << " ms\n";
    }
    return 0;
}
