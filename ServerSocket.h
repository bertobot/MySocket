/////////////////////////////////////////////////
// ServerSocket.h
//
// robert beatty
// funkejunx@aim.com
//
// this code is GPL'd
//
//

#ifndef __ServerSocket_h_
#define __ServerSocket_h_
/////////////////////////////////////////////////
#include "ClientSocket.h"
/////////////////////////////////////////////////
class ServerSocket : public Socket {
public:
    ServerSocket(int p);
    ServerSocket(int po, int d, int t, int p, int s);

    // accessors

    // functionality
    bool listen(int);
    Socket accept();
    bool isBound();
    int getPort();

    virtual ~ServerSocket();

private:
    void init(int);

protected:
    int port;
    int id;
    int connections;

    struct sockaddr_in my_sockaddr;
};
/////////////////////////////////////////////////
#endif
