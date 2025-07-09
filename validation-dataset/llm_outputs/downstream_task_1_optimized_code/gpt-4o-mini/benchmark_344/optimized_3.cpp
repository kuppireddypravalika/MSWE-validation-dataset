#include <boost/asio.hpp>
#include <functional>
#include <cstdint>
#include <atomic>

uint64_t run(std::size_t iterations)
{
    boost::asio::io_context io;
    std::atomic<std::size_t> counter(0);
    std::function<void()> handler;
    handler = [&](){
        counter.fetch_add(1, std::memory_order_relaxed);
        if (counter < iterations)
            io.post(handler);
    };
    io.post(handler);
    io.run();
    return counter.load(std::memory_order_relaxed);
}

// Explicit template instantiation statements (unchanged)
