// Original implementation: scans bits sequentially
int bitscanforward(int source) {
    for (int i = 0; i < 32; ++i) {
        if (source & (1 << i))
            return i;
    }
    return -1;
}
