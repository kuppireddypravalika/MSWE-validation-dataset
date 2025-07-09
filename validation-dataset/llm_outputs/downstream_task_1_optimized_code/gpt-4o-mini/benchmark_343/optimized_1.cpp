int prepare_op(int size, int repeat) {
    // Pre-calculate threshold for performance for large sizes
    const int threshold = 1000;

    int sum = 0;
    if (size > threshold) {
        // Use a more efficient way of calculating sum for larger sizes
        sum = (size * (size - 1)) / 2;
    } else {
        // Small sizes, default to a simple loop as in the original
        for (int i = 0; i < size; ++i) {
            sum += repeat;
        }
    }
    return sum;
}