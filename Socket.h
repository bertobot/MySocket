/////////////////////////////////////////////////
// Socket.h
//
// robert beatty
// funkejunx@aim.com
//
// this code is GPL'd
//
//

#ifndef __Socket_h_
#define __Socket_h_
/////////////////////////////////////////////////
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>

#include <string>
/////////////////////////////////////////////////
class Socket {
public:
    Socket();
    Socket(int);
    Socket(int, int, int, int);
    Socket(const Socket *&);

    // mutators
    void setDomain(int);
    void setType(int);
    void setProtocol(int);
    void setShutdownMethod(int);

    // accessors
    int getDomain();
    int getType();
    int getProtocol();
    int getShutdownMethod();

    // methods
    std::string read();
    int write(const std::string&);

    virtual bool isValid();

    void close();

    virtual ~Socket();

private:
    void init(int d=AF_INET, int t=SOCK_STREAM, int p=0, int s=SHUT_RDWR);

protected:
    int
        socket_descriptor,
        domain,
        type,
        protocol,
        shutdown_method;

    bool error;
};
/////////////////////////////////////////////////
#endif
