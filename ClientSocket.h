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
    ClientSocket(const std::string&, int);
    ClientSocket(const std::string&, int, int, int, int, int);

    bool connect();
    bool isConnected();

    virtual ~ClientSocket();

private:
    void init(const std::string&, int);

protected:
    std::string server_address;
    int port;
    int id;

    struct sockaddr_in my_sockaddr;
    struct hostent *server;
};
/////////////////////////////////////////////////
#endif
