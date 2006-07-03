#include "ServerSocket.h"
/////////////////////////////////////////////////
void ServerSocket::init(int po) {
    port = po;

    bzero( (char*)&my_sockaddr, sizeof(my_sockaddr) );

    my_sockaddr.sin_family = domain;
    my_sockaddr.sin_addr.s_addr = INADDR_ANY;
    my_sockaddr.sin_port = htons(port);

    id = bind(socket_descriptor, (struct sockaddr *)&my_sockaddr, sizeof(my_sockaddr) );
}
/////////////////////////////////////////////////
bool ServerSocket::isBound() {
    return !(id < 0);
}
/////////////////////////////////////////////////
ServerSocket::ServerSocket(int po, int d, int t, int p, int s) : Socket (d, t, p, s) {
    init(po);
}
/////////////////////////////////////////////////
ServerSocket::ServerSocket(int p) : Socket () {
    init(p);
}
/////////////////////////////////////////////////
bool ServerSocket::listen(int num) {
    int l = ::listen(socket_descriptor, num);
    if (l < 0)
        return false;
    return true;
}
/////////////////////////////////////////////////
Socket ServerSocket::accept() {
    struct sockaddr_in client_sockaddr;
    int size = sizeof(client_sockaddr);
    int nfd = ::accept(socket_descriptor, (struct sockaddr *)&client_sockaddr, (socklen_t *)&size );
    Socket socket(nfd);
    return socket;
}
/////////////////////////////////////////////////
bool ServerSocket::isValid() {
    return (isBound() && Socket::isValid() );
}
/////////////////////////////////////////////////
ServerSocket::~ServerSocket() {

}
/////////////////////////////////////////////////
