#include "node.hpp"

// Optimized version only needs two insert operations
OrderedNode* movshdup(OrderedNode* src) {
    OrderedNode* result = VInsElement(16, 4, 2, 3, src, src);
    result = VInsElement(16, 4, 0, 1, result, src);
    return result;
}
