#include <boost/asio.hpp>
#include <cstdint>

uint64_t run(std::size_t iterations)
{
    // Use a single thread for the io_context to avoid overhead of threading
    boost::asio::io_context io;
    std::atomic<std::size_t> counter(0);
    auto handler = [&](){
        if (counter.fetch_add(1) < iterations)
            io.post(handler); // Schedule next iteration
    };

    io.post(handler);
    io.run();
    return counter.load();
}

// Explicit template instantiation statements
