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
            #pragma omp critical
            {
                for (int j = 0; j < hash_lengths[i]; j++) {
                    #pragma omp atomic
                    read_hash_to_depth[hashes[i][j]]++;
                }
            }
        } else if (doReferenceDepth) {
            std::set<hash_t> sample_set(hashes[i], hashes[i] + hash_lengths[i]);
            #pragma omp critical
            {
                for (auto x : sample_set) {
                    #pragma omp atomic
                    ref_to_sample_depth[x]++;
                }
            }
        }
        delete[] hashes[i];
    }
}

