#pragma once
struct OrderedNode {
    float v[4];
};

inline OrderedNode* VInsElement(int, int, int destIdx, int srcIdx,
                               OrderedNode* dest, OrderedNode* src) {
    OrderedNode* out = new OrderedNode(*dest); // copy existing values
    out->v[destIdx] = src->v[srcIdx];          // insert selected element
    return out;
}
