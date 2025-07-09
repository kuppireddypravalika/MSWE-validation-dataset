#include <string>
#include <vector>
#include <queue>
#include <algorithm>

// Compute reverse complement of a DNA string
static std::string reverse_complement(const std::string& seq) {
    std::string result;
    result.reserve(seq.size()); // Reserve space ahead of time
    for (char c : seq) {
        switch (c) {
            case 'A': result.push_back('T'); break;
            case 'a': result.push_back('t'); break;
            case 'T': result.push_back('A'); break;
            case 't': result.push_back('a'); break;
            case 'C': result.push_back('G'); break;
            case 'c': result.push_back('g'); break;
            case 'G': result.push_back('C'); break;
            case 'g': result.push_back('c'); break;
            default:  result.push_back(c); break;
        }
    }
    return result;
}

static std::string reverse_str(const std::string& seq) {
    std::string copy(seq);
    std::reverse(copy.begin(), copy.end());
    return copy;
}

// Build a heap of canonical kmers (lexicographically smaller between kmer and reverse complement)
std::priority_queue<std::string> kmer_heap(const std::string& seq, const std::vector<int>& kmer) {
    std::priority_queue<std::string> ret;
    for (int k : kmer) {
        for (int i = 0; i <= static_cast<int>(seq.length()) - k; ++i) {
            std::string forward = seq.substr(i, k);
            std::string revrev = reverse_complement(forward);
            ret.push(revrev < forward ? revrev : forward);
        }
    }
    return ret;
}

// Explicit template instantiations
