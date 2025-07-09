#include <boost/asio.hpp>
#include <functional>
#include <cstdint>

// Simulate NetworkThread without explicit concurrency hint
uint64_t run(std::size_t iterations)
{
    boost::asio::io_context io; // default constructor
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
