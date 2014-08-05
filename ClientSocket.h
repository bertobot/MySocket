/////////////////////////////////////////////////
// ClientSocket.h
//
// robert beatty
// funkejunx@aim.com
//
// this code is GPL'd
//
//

#ifndef __ClientSocket_h_
#define __ClientSocket_h_
/////////////////////////////////////////////////
#include "Socket.h"
/////////////////////////////////////////////////
class ClientSocket : public Socket {
public:
    ClientSocket();
    ClientSocket(const std::string&, int);
    ClientSocket(const std::string&, const std::string&);
    ClientSocket(const std::string&, int, struct hostent*);
    ClientSocket(const std::string&, int, int, int, int, int);

    bool connect();
    bool connect2();
    bool isConnected();

	struct hostent * getServer();

    // TODO: implement
    
    int recvfrom();
    int sendto();

    virtual ~ClientSocket();

private:
    void init(const std::string&, int);
    void setSockAddr(const std::string&, const std::string&);

    std::string server_address;
    int port;
    int id;

    struct sockaddr_in my_sockaddr;
    struct hostent *server;
    addrinfo *addr_result;
};
/////////////////////////////////////////////////
#endif
