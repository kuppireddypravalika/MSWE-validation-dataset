#include <boost/asio.hpp>
#include <functional>
#include <cstdint>

uint64_t run(std::size_t iterations)
{
    boost::asio::io_context io;
    std::atomic<std::size_t> counter(0);
    std::function<void()> handler;
    handler = [&](){
        if (counter.fetch_add(1) < iterations)
            io.post(handler);
    };
    io.post(handler);
    io.run();
    return counter.load();
}

