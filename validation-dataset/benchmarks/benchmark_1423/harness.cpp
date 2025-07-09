#include <chrono>
#include <iostream>
#include <string>
#include <vector>
#include <random>
#include <algorithm>
#include <queue>
#ifdef _OPENMP
#include <omp.h>
#endif

std::priority_queue<std::string> kmer_heap(const std::string&, const std::vector<int>&);

// Generate random DNA sequence of length n
static std::string make_sequence(int n) {
    const char bases[] = {'A','C','G','T'};
    std::mt19937 gen(42);
    std::uniform_int_distribution<int> dist(0,3);
    std::string seq(n,'A');
    for(int i=0;i<n;++i) seq[i] = bases[dist(gen)];
    return seq;
}

int main(int argc, char* argv[]) {
    std::string mode = "perf";
    int length = 5000;
    int repeat = 1500;
    int threads = 4;

    for(int i=1;i<argc;++i){
        std::string arg = argv[i];
        if(arg=="--mode=correct") mode = "correct";
        else if(arg=="--mode=perf") mode = "perf";
        else if(arg.rfind("--length=",0)==0) length = std::stoi(arg.substr(9));
        else if(arg.rfind("--repeat=",0)==0) repeat = std::stoi(arg.substr(9));
        else if(arg.rfind("--threads=",0)==0) threads = std::stoi(arg.substr(10));
    }

#ifdef _OPENMP
    threads = std::min(threads, omp_get_max_threads());
    omp_set_num_threads(threads);
#endif

    std::string seq = make_sequence(length);
    std::vector<int> kmer_sizes{10,15,20};

    if(mode=="correct") {
        auto pq = kmer_heap(seq, kmer_sizes);
        size_t sum = 0;
        while(!pq.empty()) { sum += pq.top().size(); pq.pop(); }
        std::cout << sum << "\n";
    } else {
        using clock = std::chrono::high_resolution_clock;
        auto t1 = clock::now();
        size_t dummy = 0;
        for(int r=0;r<repeat;++r) {
            auto pq = kmer_heap(seq, kmer_sizes);
            dummy += pq.size();
        }
        auto t2 = clock::now();
        std::cout << "Time: " << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count() << " ms\n";
        if(dummy==123456789) std::cerr << "";
    }
    return 0;
}
