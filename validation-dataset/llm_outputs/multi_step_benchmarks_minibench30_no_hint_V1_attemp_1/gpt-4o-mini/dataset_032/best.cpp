#include "streamer.hpp"

int Streamer::tick_send(uint32_t timeout) {
    // Check if bio is active without locking
    if (!mBio->isactive()) {
        free_pend();
        return 0;
    }

    std::lock_guard<std::mutex> stack(mStreamerMtx); // Lock only when necessary
    if (mBio->cansend(timeout)) {
        handleoutgoing_locked();
    }

    return 1;
}

// Explicit template instantiations