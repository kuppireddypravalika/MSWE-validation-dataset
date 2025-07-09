#include <chrono>
#include <iostream>
#include <string>
#include <vector>
#include <memory>

struct Object { virtual ~Object() = default; };
struct TcpSocket : Object {
    int descriptor;
    explicit TcpSocket(int d) : descriptor(d) {}
    int socketDescriptor() const { return descriptor; }
};
using LocalUnixSocket = TcpSocket;

struct LocalServer : Object {
    std::vector<Object*> childs;
    const std::vector<Object*>& children() const { return childs; }
    void addChild(Object* o) { childs.push_back(o); }
};

// Declare functions
bool setSocketDescriptor(LocalServer* server, int descriptor,
                         LocalUnixSocket*& out);

// Utility to build a server with n child sockets
std::unique_ptr<LocalServer> createServer(int n) {
    auto server = std::make_unique<LocalServer>();
    for (int i = 0; i < n; ++i)
        server->addChild(new LocalUnixSocket(i));
    return server;
}

long long run_query(LocalServer* server, const std::vector<int>& queries) {
    LocalUnixSocket* out = nullptr;
    long long sum = 0;
    for (int q : queries)
        if (setSocketDescriptor(server, q, out))
            sum += out->socketDescriptor();
    return sum;
}

int main(int argc, char* argv[]) {
    std::string mode = "perf";
    int repeat = 100;
    int children = 5000;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--mode=correct") mode = "correct";
        else if (arg == "--mode=perf") mode = "perf";
        else if (arg.rfind("--repeat=",0)==0) repeat = std::stoi(arg.substr(9));
        else if (arg.rfind("--children=",0)==0) children = std::stoi(arg.substr(11));
    }

    auto server = createServer(children);
    std::vector<int> queries(children);
    for (int i = 0; i < children; ++i) queries[i] = i;

    if (mode == "correct") {
        long long sum = run_query(server.get(), queries);
        std::cout << sum << "\n";
    } else {
        using clock = std::chrono::high_resolution_clock;
        auto t1 = clock::now();
        for (int r = 0; r < repeat; ++r)
            run_query(server.get(), queries);
        auto t2 = clock::now();
        std::cout << "Time: "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()
                  << " ms\n";
    }
    // cleanup
    for (Object* o : server->children()) delete o;
    return 0;
}
