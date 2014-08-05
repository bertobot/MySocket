#include "ServerSocket.h"
/////////////////////////////////////////////////
void ServerSocket::init(int po) {
    port = po;

    bzero( (char*)&my_sockaddr, sizeof(my_sockaddr) );

    my_sockaddr.sin_family = getDomain();
    my_sockaddr.sin_addr.s_addr = INADDR_ANY;
    my_sockaddr.sin_port = htons(port);

    // set to always reuse addr.
    int boolean = 1;

    if (setsockopt(getSocketDescriptor(), SOL_SOCKET, SO_REUSEADDR, &boolean, sizeof(int) ) == 0)
	id = bind(getSocketDescriptor(), (struct sockaddr *)&my_sockaddr, sizeof(my_sockaddr) );
    else
	id = -1;

}
/////////////////////////////////////////////////
bool ServerSocket::isBound() {
    return !(id < 0);
}
/////////////////////////////////////////////////
ServerSocket::ServerSocket(int po, int t, int d, int p, int s) : Socket (t, d, p, s) {
    init(po);
}
/////////////////////////////////////////////////
ServerSocket::ServerSocket(int p) : Socket () {
    init(p);
}
/////////////////////////////////////////////////
bool ServerSocket::listen(int num) {
    int l = ::listen(getSocketDescriptor(), num);
    if (l < 0)
        return false;
    return true;
}
/////////////////////////////////////////////////
Socket ServerSocket::accept() {
    struct sockaddr_in client_sockaddr;
    int size = sizeof(client_sockaddr);
    int nfd = ::accept(getSocketDescriptor(), (struct sockaddr *)&client_sockaddr, (socklen_t *)&size );
    Socket socket(nfd);
    return socket;
}
/////////////////////////////////////////////////
int ServerSocket::getPort() {
    return port;
}
/////////////////////////////////////////////////
ServerSocket::~ServerSocket() {

}
/////////////////////////////////////////////////
