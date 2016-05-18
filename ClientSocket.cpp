#include "ClientSocket.h"
/////////////////////////////////////////////////
ClientSocket::ClientSocket() : Socket() {
	server = NULL;
    addr_result = NULL;
    mConnected = false;
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
    mConnected = false;
}
/////////////////////////////////////////////////
ClientSocket::ClientSocket(const std::string& address,
            int po,
            int t,
            int d,            
            int p,
            int s) : Socket(t, d,  p, s) {

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
			setError(true);
			return;
		}

		my_sockaddr.sin_family = getDomain();

		bcopy(
			(char *)server->h_addr,
			(char *)&my_sockaddr.sin_addr.s_addr,
			server->h_length );
	}

    my_sockaddr.sin_port = htons(port);

    mConnected = false;
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
            sfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
            rp = rp->ai_next;
        }

        setSocketDescriptor(sfd);
    }
}
/////////////////////////////////////////////////
bool ClientSocket::connect(double timeout) {
    setBlocking(false);
    int id = ::connect(getSocketDescriptor(), (sockaddr *)&my_sockaddr, sizeof(my_sockaddr) );

    if (id < 0) {
        if (errno == EINPROGRESS) {
            fd_set ready;
            struct timeval ts;
    
            ts.tv_sec = long(timeout);
            ts.tv_usec = long((timeout - ts.tv_sec) * 1000000);

            FD_ZERO(&ready);
            FD_SET(getSocketDescriptor(), &ready);
    
            int nfds = getSocketDescriptor() + 1;

            if (select(nfds, NULL, &ready, NULL, &ts) > 0) {
                int valopt;
                socklen_t lon = sizeof(int);

                if (getsockopt(getSocketDescriptor(), SOL_SOCKET, SO_ERROR, (void*)(&valopt), &lon) < 0)
                    return false;

                if (valopt) {
                    errno = valopt;
                    return false;
                }

                // everything is good at this point
                setBlocking(true);
                mConnected = true;
                return true;
            }
            else
                errno = ETIMEDOUT;
        }
    }

    return false;
}
/////////////////////////////////////////////////
bool ClientSocket::connect2(double timeout) {
    if (!addr_result)
        return false;

    return connect(timeout);
}
/////////////////////////////////////////////////
struct hostent * ClientSocket::getServer() {
	return server;
}
/////////////////////////////////////////////////
// TODO: implement sendto and recvfrom
/////////////////////////////////////////////////
ClientSocket::~ClientSocket() {
    if (addr_result)
        freeaddrinfo(addr_result);
}
/////////////////////////////////////////////////
