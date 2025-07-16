// original.cpp
#include <string>
#include <vector>
#include <queue>
#include <algorithm>

// Compute reverse complement of a DNA string
static std::string reverse_complement(const std::string& seq) {
    std::string ret;
    ret.reserve(seq.size()); // Reserve space to avoid multiple allocations
    for (char c : seq) {
        switch (c) {
            case 'A': ret += 'T'; break;
            case 'a': ret += 't'; break;
            case 'T': ret += 'A'; break;
            case 't': ret += 'a'; break;
            case 'C': ret += 'G'; break;
            case 'c': ret += 'g'; break;
            case 'G': ret += 'C'; break;
            case 'g': ret += 'c'; break;
            default:  ret += c; break;
        }
    }
    return ret;
}

// Build a heap of canonical kmers (lexicographically smaller between kmer and reverse complement)
std::priority_queue<std::string> kmer_heap(const std::string& seq, const std::vector<int>& kmer) {
    std::priority_queue<std::string> ret;
    size_t seq_length = seq.length(); // Cache the sequence length

    for (int k : kmer) {
        for (size_t i = 0; i <= seq_length - k; ++i) {
            std::string forward = seq.substr(i, k);
            std::string revrev = reverse_complement(forward);
            // Only push if the forward string is greater than the reverse complement string
            if (revrev < forward) {
                ret.push(revrev);
            } else {
                ret.push(forward);
            }
        }
    }
    return ret;
}

