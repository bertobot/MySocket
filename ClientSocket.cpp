#include "ClientSocket.h"
/////////////////////////////////////////////////
ClientSocket::ClientSocket() : Socket() {
	server = NULL;
}
/////////////////////////////////////////////////
ClientSocket::ClientSocket(const std::string& addr, int po) : Socket() {
    server = NULL;
    init(addr, po);
}
/////////////////////////////////////////////////
ClientSocket::ClientSocket(const std::string& addr, int po, struct hostent*s) : Socket() {
    server = s;
    init(addr, po);
}
/////////////////////////////////////////////////
ClientSocket::ClientSocket(const std::string& address,
            int po,
            int d,
            int t,
            int p,
            int s) : Socket(d, t, p, s) {

    server = NULL;
    init(address, po);
}
/////////////////////////////////////////////////
void ClientSocket::init(const std::string& addr, int p) {
    server_address = addr;
    port = p;

    bzero( (char*)&my_sockaddr, sizeof(my_sockaddr) );

	if (! server) {
		server = gethostbyname(server_address.c_str() );
		if (! server) {
			error = true;
			return;
		}

		my_sockaddr.sin_family = domain;
		bcopy(
			(char *)server->h_addr,
			(char *)&my_sockaddr.sin_addr.s_addr,
			server->h_length );
	}

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
struct hostent * ClientSocket::getServer() {
	return server;
}
/////////////////////////////////////////////////
ClientSocket::~ClientSocket() {

}
/////////////////////////////////////////////////
