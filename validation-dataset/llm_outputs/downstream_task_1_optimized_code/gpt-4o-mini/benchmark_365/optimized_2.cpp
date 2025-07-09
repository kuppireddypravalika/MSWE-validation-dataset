#include <vector>
#include <cstddef>
#include <numeric> // for std::accumulate

struct LoadNode {
    using Float = float;
    unsigned int m_Point;
    std::vector<Float> m_Force;
    void* m_Element;

    LoadNode(void* element_, unsigned int pt_, std::vector<Float> F_)
        : m_Point(pt_), m_Force(std::move(F_)) // use move semantics directly
    {
        m_Element = element_;
    }
};

std::size_t run(int repeat, std::size_t vec_size)
{
    std::vector<float> vec(vec_size, 1.0f);
    std::size_t sum = 0;

    for (int i = 0; i < repeat; ++i)
    {
        LoadNode node(nullptr, i, vec);
        sum += node.m_Force[i % vec_size]; // simplified indexing directly with vec_size
    }

    return sum;
}

// Explicit template instantiation statements
