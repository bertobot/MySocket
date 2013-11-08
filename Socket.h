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
#include <fcntl.h>

#include <string.h>
#include <string>
/////////////////////////////////////////////////
class Socket {
public:
    Socket();
    Socket(int descriptor);
    Socket(int type, int domain, int protocol, int shutdownMethod);
    Socket(const Socket&);
    Socket operator =(const Socket &rhs);

    // mutators
    void setDomain(int);
    void setType(int);
    void setProtocol(int);
    void setShutdownMethod(int);

    // accessors
    int getSocketDescriptor();

    int getDomain();
    int getType();
    int getProtocol();
    int getShutdownMethod();

    // methods

    // read

    int read(char *buffer, int size);
    int read(char *buffer, int size, int flags);
    std::string read(int size);
    char readByte();
    char readByte(int flags);
    std::string readLine();

    // write

    int write(char *buffer, int size);
    int write(char *buffer, int size, int flags);
    int write(const std::string& str);
    int write(const std::string& str, int flags);
    
    int writeByte(char c);
    int writeByte(char c, int flags);

    int writeLine(const std::string& str, int flags=MSG_NOSIGNAL);

    int setOption(int level, int optname, int boolean);

    bool isValid();

    std::string getClientIP();
    long getClientPort();

    int close();
    int makeNonBlocking();

    int getDebug();
    void setDebug(int d);

    


    virtual ~Socket();

private:
    void init(int d=AF_INET, int t=SOCK_STREAM, int p=0, int s=SHUT_RDWR);
    void pre_init(int d=AF_INET, int t=SOCK_STREAM, int p=0, int s=SHUT_RDWR);

protected:
    int
        socket_descriptor,
        domain,
        type,
        protocol,
        shutdown_method;

    bool error;
    int _debug;
};
/////////////////////////////////////////////////
#endif
