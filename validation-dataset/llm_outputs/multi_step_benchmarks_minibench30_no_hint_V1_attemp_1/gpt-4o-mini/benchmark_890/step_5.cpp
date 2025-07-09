// hash_common.hpp
#pragma once
#include <vector>
#include <unordered_map>
#include <tuple>
#include <unordered_set>
#include <cstdint>
#include <cstring>
#include <string>

using hash_t = uint64_t;

inline std::tuple<hash_t*, int>
allhash_unsorted_64_fast(const char* seq, int len, const std::vector<int>& kmer) {
    hash_t* arr = new hash_t[len];
    uint64_t h = 14695981039346656037ull;
    for (int i = 0; i < len; ++i) {
        h ^= static_cast<unsigned char>(seq[i % len]);
        h *= 1099511628211ull;
        arr[i] = h % 16384;
    }
    return {arr, len};
}

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


// original.cpp
#include "hash_common.hpp"
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
        bool doReferenceDepth) {

    #pragma omp parallel for
    for (int i = 0; i < static_cast<int>(keys.size()); i++) {
        auto hashes_and_num = allhash_unsorted_64_fast(seqs[i], lengths[i], kmer);
        hashes[i] = std::get<0>(hashes_and_num);
        hash_lengths[i] = std::get<1>(hashes_and_num);
        if (doReadDepth) {
            for (int j = 0; j < hash_lengths[i]; j++) {
                #pragma omp atomic
                read_hash_to_depth[hashes[i][j]]++;
            }
        } else if (doReferenceDepth) {
            std::unordered_set<hash_t> sample_set(hashes[i], hashes[i] + hash_lengths[i]);
            for (const auto& x : sample_set) {
                #pragma omp atomic
                ref_to_sample_depth[x]++;
            }
        }
        delete[] hashes[i];
    }
}