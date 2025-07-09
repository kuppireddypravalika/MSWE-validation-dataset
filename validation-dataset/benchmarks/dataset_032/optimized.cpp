#include "streamer.hpp"

int Streamer::tick_send(uint32_t timeout)
{
    /* short circuit everything if bio isn't active */
    if (!(mBio->isactive()))
    {
        free_pend();
        return 0;
    }

    if (mBio->cansend(timeout))
    {
        std::lock_guard<std::mutex> stack(mStreamerMtx);
        handleoutgoing_locked();
    }

    return 1;
}
