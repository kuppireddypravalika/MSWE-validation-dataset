#include <string>
#include <vector>
#include <queue>
#include <algorithm>
#include <sstream>
#ifdef _OPENMP
#include <omp.h>
#endif

static std::string reverse_complement(const std::string& seq) {
    std::stringstream ret;
    for(char c : seq) {
        switch(c) {
            case 'A': ret << 'T'; break;
            case 'a': ret << 't'; break;
            case 'T': ret << 'A'; break;
            case 't': ret << 'a'; break;
            case 'C': ret << 'G'; break;
            case 'c': ret << 'g'; break;
            case 'G': ret << 'C'; break;
            case 'g': ret << 'c'; break;
            default:  ret << c;  break;
        }
    }
    return ret.str();
}

static std::string reverse_str(const std::string& seq) {
    std::string copy(seq);
    std::reverse(copy.begin(), copy.end());
    return copy;
}

std::priority_queue<std::string> kmer_heap(const std::string& seq, const std::vector<int>& kmer) {
    std::vector<std::string> base;
    for(int k : kmer) {
        std::vector<std::string> out(seq.length() - k, "");
#pragma omp parallel for
        for(int i = 0; i < (int)seq.length() - k; ++i) {
            std::string forward = seq.substr(i, k);
            std::string revrev = reverse_str(reverse_complement(forward));
            out[i] = (revrev < forward ? revrev : forward);
        }
        base.reserve(base.size() + out.size());
        base.insert(base.end(), out.begin(), out.end());
    }
    std::priority_queue<std::string> ret(base.begin(), base.end());
    return ret;
}
