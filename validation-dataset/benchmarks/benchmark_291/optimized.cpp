#include <vector>

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

static bool setSocketDescriptor_impl(LocalServer* server, int descriptor,
                                     LocalUnixSocket*& out) {
    if (server) {
        for (Object* child : server->children()) {
            TcpSocket* tcp = dynamic_cast<TcpSocket*>(child);
            if (tcp && tcp->socketDescriptor() == descriptor) {
                out = static_cast<LocalUnixSocket*>(tcp);
                return true;
            }
        }
    }
    return false;
}

bool setSocketDescriptor(LocalServer* server, int descriptor,
                         LocalUnixSocket*& out) {
    return setSocketDescriptor_impl(server, descriptor, out);
}
