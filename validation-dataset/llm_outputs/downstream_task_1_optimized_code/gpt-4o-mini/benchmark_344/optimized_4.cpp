#include <boost/asio.hpp>
#include <functional>
#include <cstdint>

uint64_t run(std::size_t iterations)
{
    boost::asio::io_context io;
    std::size_t counter = 0;
    // Reserve memory for better performance
    io.set_thread_count(std::thread::hardware_concurrency());
    std::function<void()> handler = [&](){
        for (std::size_t i = 0; i < iterations; ++i) {
            ++counter;
            io.post([&]{ ++counter; }); // schedule next iteration
        }
    };
    io.post(handler);
    io.run();
    return counter;
}

// Explicit template instantiation statements
// Add your explicit template instantiation statements here.