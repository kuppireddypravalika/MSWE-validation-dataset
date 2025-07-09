#include "hash_common.hpp"
#include <chrono>
#include <iostream>
#include <random>
#include <string>
#include <omp.h>

void hash_sequences(std::vector<std::string>& keys,
        std::vector<char*>& seqs,
        std::vector<int>& lengths,
        std::vector<hash_t*>& hashes,
        std::vector<int>& hash_lengths,
        std::vector<int>& kmer,
        std::unordered_map<hash_t,int>& read_hash_to_depth,
        std::unordered_map<hash_t,int>& ref_to_sample_depth,
        bool doReadDepth,
        bool doReferenceDepth);

std::vector<char*> make_seqs(int count, int len) {
    std::vector<char*> seqs(count);
    std::mt19937 gen(42);
    std::uniform_int_distribution<int> dist(0,3);
    const char bases[4] = {'A','C','G','T'};
    for (int i=0;i<count;++i) {
        seqs[i] = new char[len];
        for (int j=0;j<len;++j) seqs[i][j] = bases[dist(gen)];
    }
    return seqs;
}

int main(int argc, char* argv[]) {
    std::string mode = "perf";
    int seq_count = 400;
    int seq_len = 256;
    int repeat = 1000;

    for (int i=1;i<argc;++i) {
        std::string arg = argv[i];
        if (arg == "--mode=correct") mode = "correct";
        else if (arg == "--mode=perf") mode = "perf";
        else if (arg.rfind("--seqs=",0)==0) seq_count = std::stoi(arg.substr(7));
        else if (arg.rfind("--len=",0)==0) seq_len = std::stoi(arg.substr(6));
        else if (arg.rfind("--repeat=",0)==0) repeat = std::stoi(arg.substr(9));
    }

    std::vector<std::string> keys(seq_count);
    for (int i=0;i<seq_count;++i) keys[i] = "s" + std::to_string(i);
    std::vector<char*> seqs = make_seqs(seq_count, seq_len);
    std::vector<int> lengths(seq_count, seq_len);
    std::vector<hash_t*> hashes(seq_count);
    std::vector<int> hash_lengths(seq_count);
    std::vector<int> kmer = {3};
    std::unordered_map<hash_t,int> read_hash_to_depth;
    std::unordered_map<hash_t,int> ref_to_sample_depth;

    if (mode == "correct") {
        hash_sequences(keys, seqs, lengths, hashes, hash_lengths, kmer,
                       read_hash_to_depth, ref_to_sample_depth, true, false);
        long long sum = 0;
        for (auto& kv : read_hash_to_depth) sum += kv.second;
        std::cout << sum << '\n';
    } else {
        using clock = std::chrono::high_resolution_clock;
        long long dummy = 0;
        auto t1 = clock::now();
        for (int r=0; r<repeat; ++r) {
            hash_sequences(keys, seqs, lengths, hashes, hash_lengths, kmer,
                           read_hash_to_depth, ref_to_sample_depth, true, false);
            dummy += read_hash_to_depth.begin()->second;
        }
        auto t2 = clock::now();
        std::cout << "Time: "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()
                  << " ms\n";
        if (dummy == 123456789) std::cerr << "";
    }

    for (char* s : seqs) delete[] s;
    return 0;
}

