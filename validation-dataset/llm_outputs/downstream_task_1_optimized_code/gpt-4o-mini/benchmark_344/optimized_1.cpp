#include <boost/asio.hpp>
#include <functional>
#include <cstdint>

uint64_t run(std::size_t iterations)
{
    boost::asio::io_context io;
    std::size_t counter = 0;
    // Reserve space for potential optimizations
    io.post([&counter, iterations, &io](){
        do {
            ++counter;
        } while (counter < iterations && io.run_one() > 0);
    });
    io.run();
    return counter;
}