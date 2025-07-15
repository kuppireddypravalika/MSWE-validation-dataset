#include "hash_common.hpp"
#include <omp.h>
#include <vector>
#include <string>
#include <unordered_map>
#include <iostream>

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

int main() {
    std::vector<std::string> keys = {"key1", "key2"};
    std::vector<char*> seqs = {strdup("ACGT"), strdup("TGCA")};
    std::vector<int> lengths = {4, 4};
    std::vector<hash_t*> hashes(2);
    std::vector<int> hash_lengths(2);
    std::vector<int> kmer = {1, 2, 3};
    std::unordered_map<hash_t, int> read_hash_to_depth;
    std::unordered_map<hash_t, int> ref_to_sample_depth;

    hash_sequences(keys, seqs, lengths, hashes, hash_lengths, kmer, read_hash_to_depth, ref_to_sample_depth, true, false);

    for (auto& kv : read_hash_to_depth) {
        std::cout << kv.first << ": " << kv.second << std::endl;
    }

    for (auto seq : seqs) {
        free(seq);
    }

    return 0;
}
