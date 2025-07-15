#include <string>
#include <vector>
#include <queue>
#include <algorithm>
#include <sstream>
#include <iostream>
// Compute reverse complement of a DNA string
static std::string reverse_complement(const std::string& seq) {
    static char complement[256] = {0};
    static bool initialized = false;
    if (!initialized) {
        complement['A'] = 'T';
        complement['a'] = 't';
        complement['T'] = 'A';
        complement['t'] = 'a';
        complement['C'] = 'G';
        complement['c'] = 'g';
        complement['G'] = 'C';
        complement['g'] = 'c';
        initialized = true;
    }
    std::string ret;
    ret.reserve(seq.size());
    for(char c : seq) {
        ret += complement[(unsigned char)c];
    }
    return ret;
}

static std::string reverse_str(const std::string& seq) {
    std::string copy(seq);
    std::reverse(copy.begin(), copy.end());
    return copy;
}

// Build a heap of canonical kmers (lexicographically smaller between kmer and reverse complement)
std::priority_queue<std::string> kmer_heap(const std::string& seq, const std::vector<int>& kmer) {
    std::priority_queue<std::string> ret;
    for(int k : kmer) {
        for(int i = 0; i < (int)seq.length() - k; ++i) {
            std::string forward = seq.substr(i, k);
            std::string revrev = reverse_str(reverse_complement(forward));
            ret.push(revrev < forward ? revrev : forward);
        }
    }
    return ret;
}
