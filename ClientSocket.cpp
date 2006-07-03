#include "ClientSocket.h"
/////////////////////////////////////////////////
ClientSocket::ClientSocket(const std::string& addr, int po) : Socket() {
    init(addr, po);
}
/////////////////////////////////////////////////
ClientSocket::ClientSocket(const std::string& address,
            int po,
            int d,
            int t,
            int p,
            int s) : Socket(d, t, p, s) {

    init(address, po);
}
/////////////////////////////////////////////////
void ClientSocket::init(const std::string& addr, int p) {
    server_address = addr;
    port = p;

    bzero( (char*)&my_sockaddr, sizeof(my_sockaddr) );
    server = gethostbyname(server_address.c_str() );

    my_sockaddr.sin_family = domain;
    bcopy(
        (char *)server->h_addr,
        (char *)&my_sockaddr.sin_addr.s_addr,
        server->h_length );

    my_sockaddr.sin_port = htons(port);
}
/////////////////////////////////////////////////
bool ClientSocket::connect() {
    id = ::connect(socket_descriptor, (sockaddr *)&my_sockaddr, sizeof(my_sockaddr) );
    return isConnected();
}
/////////////////////////////////////////////////
bool ClientSocket::isConnected() {
    return !(id < 0);
}
/////////////////////////////////////////////////
ClientSocket::~ClientSocket() {

}
/////////////////////////////////////////////////
