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

    /**
    * connect to remote host.
    * @param timeout number of seconds to try.
    * @return bool true if connects before timeout, false otherwise.
    */
    bool connect(double timeout=180);
    bool connect2(double timeout=180);
    bool isConnected() { return mConnected; }

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

    bool mConnected;
};
/////////////////////////////////////////////////
#endif
