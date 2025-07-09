#include <chrono>
#include <iostream>
#include <string>
#include <vector>

struct Position {
    bool advanced_pawn_push(int move) const { return move % 7 == 0; }
    int see_sign(int move) const { return (move % 10) - 5; }
};

int search(const Position&, const std::vector<int>&, int);

std::vector<int> createMoves(int n) {
    std::vector<int> m(n);
    for(int i=0;i<n;++i) m[i]=i;
    return m;
}

int main(int argc, char* argv[]) {
    std::string mode = "perf";
    int repeat = 2000;
    int moves = 1000;
    int depth = 2;
    for(int i=1;i<argc;++i){
        std::string arg=argv[i];
        if(arg=="--mode=correct") mode="correct";
        else if(arg=="--mode=perf") mode="perf";
        else if(arg.rfind("--repeat=",0)==0) repeat=std::stoi(arg.substr(9));
        else if(arg.rfind("--moves=",0)==0) moves=std::stoi(arg.substr(8));
    }

    Position p;
    std::vector<int> mv = createMoves(moves);

    if(mode=="correct") {
        int res = search(p,mv,depth);
        std::cout << res << '\n';
        return 0;
    }

    using clock = std::chrono::high_resolution_clock;
    auto t1 = clock::now();
    for(int i=0;i<repeat;++i) search(p,mv,depth);
    auto t2 = clock::now();

    std::cout << "Time: " << std::chrono::duration_cast<std::chrono::milliseconds>(t2-t1).count() << " ms\n";
}
