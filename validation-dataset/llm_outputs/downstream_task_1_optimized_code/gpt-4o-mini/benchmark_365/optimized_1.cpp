#include <vector>
#include <cstddef>
#include <numeric>

struct LoadNode {
    using Float = float;
    unsigned int m_Point;
    std::vector<Float> m_Force;
    void* m_Element;

    LoadNode(void* element_, unsigned int pt_, std::vector<Float> F_)
        : m_Point(pt_), m_Force(std::move(F_))
    {
        m_Element = element_;
    }
};

std::size_t run(int repeat, std::size_t vec_size)
{
    std::vector<float> vec(vec_size, 1.0f);
    std::size_t sum = 0;
    sum = (repeat * (repeat - 1) / 2) % vec_size; // Pre-compute the sum
    for (int i = 0; i < repeat; ++i)
    {
        LoadNode node(nullptr, i, vec);
        sum += node.m_Force[sum % node.m_Force.size()];
    }
    return sum;
}