#include <boost/asio.hpp>
#include <functional>
#include <cstdint>

// Optimized version using single threaded hint
uint64_t run(std::size_t iterations)
{
    boost::asio::io_context io(1); // concurrency hint = 1
    std::size_t counter = 0;
    std::function<void()> handler;
    handler = [&](){
        ++counter;
        if (counter < iterations)
            io.post(handler); // schedule next iteration
    };
    io.post(handler);
    io.run();
    return counter;
}
