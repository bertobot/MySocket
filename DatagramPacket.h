/////////////////////////////////////////////////
// DatagramPacket.h
//
// robert beatty
// funkejunx@aim.com
//
// this code is GPL'd
//
//

#ifndef __DatagramPacket_h_
#define __DatagramPacket_h_
/////////////////////////////////////////////////
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string>
#include <string.h>
#include <strings.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
/////////////////////////////////////////////////
class DatagramPacket {
public:
    DatagramPacket();
    DatagramPacket(const std::string& addr, const std::string& port);

    void setSocketDescriptor(int sock);
    int getSocketDescriptor();

    int read(char *buffer, int size, int flags=MSG_NOSIGNAL);
    int write(char *buffer, int size, int flags=MSG_NOSIGNAL);

    void clear();
    void close();

    virtual ~DatagramPacket();

protected:
    int sock;
    std::string address;
    std::string port;
    
    
    addrinfo
	*remoteaddr,
	*addrresult;

    void init();
};
/////////////////////////////////////////////////
#endif
