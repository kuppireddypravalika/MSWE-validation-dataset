#include <vector>
#include <cstddef>

struct LoadNode {
    using Float = float;
    unsigned int m_Point;
    std::vector<Float> m_Force;
    void* m_Element;

    LoadNode(void* element_, unsigned int pt_, std::vector<Float> F_)
        : m_Point(pt_), m_Force(std::move(F_)) // move F_ into member
    {
        m_Element = element_;
    }
};

std::size_t run(int repeat, std::size_t vec_size)
{
    std::size_t sum = 0;
    std::vector<float> vec(vec_size, 1.0f); // Move vector allocation outside of the loop
    for (int i = 0; i < repeat; ++i)
    {
        LoadNode node(nullptr, i, vec); // Use the preallocated vector without copying
        sum += node.m_Force[i % node.m_Force.size()];
    }
    return sum;
}

// Explicit template instantiation statements (if needed) could go here.