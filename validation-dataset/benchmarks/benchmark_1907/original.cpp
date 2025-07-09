#include "node.hpp"

// Unoptimized implementation inserting four elements
OrderedNode* movshdup(OrderedNode* src) {
    OrderedNode* result = VInsElement(16, 4, 3, 3, src, src);
    result = VInsElement(16, 4, 2, 3, result, src);
    result = VInsElement(16, 4, 1, 1, result, src);
    result = VInsElement(16, 4, 0, 1, result, src);
    return result;
}
