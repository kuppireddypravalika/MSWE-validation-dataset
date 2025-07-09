#include "streamer.hpp"

int Streamer::tick_send(uint32_t timeout)
{
    std::lock_guard<std::mutex> stack(mStreamerMtx);

    /* short circuit everything if bio isn't active */
    if (!(mBio->isactive()))
    {
        free_pend();
        return 0;
    }

    if (mBio->cansend(timeout))
    {
        handleoutgoing_locked();
    }

    return 1;
}
