#include "ClientSocket.h"
/////////////////////////////////////////////////
ClientSocket::ClientSocket() : Socket() {
	server = NULL;
    addr_result = NULL;
}
/////////////////////////////////////////////////
ClientSocket::ClientSocket(const std::string& addr, int po) : Socket() {
    server = NULL;
    addr_result = NULL;
    init(addr, po);
}
/////////////////////////////////////////////////
ClientSocket::ClientSocket(const std::string& addr, int po, struct hostent*s) : Socket() {
    server = s;
    addr_result = NULL;
    init(addr, po);
}
/////////////////////////////////////////////////
ClientSocket::ClientSocket(const std::string& addr, const std::string &service) : Socket() {
    server = NULL;
    addr_result = NULL;
    setSockAddr(addr, service);
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
void ClientSocket::setSockAddr(const std::string &address, const std::string &service) {
    addrinfo hints;
   
    // std::string host = "localhost", port = "33001";

    memset(&hints, 0, sizeof(struct addrinfo));
    
    hints.ai_family = AF_UNSPEC;        /* Allow IPv4 or IPv6 */
    //hints.ai_socktype = SOCK_DGRAM;   /* Datagram socket */
    hints.ai_socktype = SOCK_STREAM;    /* stream socket */
    hints.ai_flags = 0;
    hints.ai_protocol = 0;              /* Any protocol */

    int rc = getaddrinfo(address.c_str(), service.c_str(), &hints, &addr_result);

    if (rc != 0) {
        // fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rc));
    }

    if (addr_result) {
        int sfd = 0;
        addrinfo *rp = addr_result;
        while (sfd < 1) {
            domain = rp->ai_family;
            type = rp->ai_socktype;
            protocol = rp->ai_protocol;

            sfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
            rp = rp->ai_next;
        }
        socket_descriptor = sfd;
    }
}
/////////////////////////////////////////////////
bool ClientSocket::connect() {
    id = ::connect(socket_descriptor, (sockaddr *)&my_sockaddr, sizeof(my_sockaddr) );
    return isConnected();
}
/////////////////////////////////////////////////
bool ClientSocket::connect2() {
    if (!addr_result)
        return false;
    
    id = ::connect(socket_descriptor, addr_result->ai_addr, addr_result->ai_addrlen);
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
    if (addr_result)
        freeaddrinfo(addr_result);
}
/////////////////////////////////////////////////
