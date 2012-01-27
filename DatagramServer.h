#ifndef __DatagramServer_h_
#define __DatagramServer_h_

#include "DatagramPacket.h"

class DatagramServer : public DatagramPacket {
public:
    DatagramServer(int port);

    bool isBound();
    void init();

    int read(char *buffer, int size, int flags = 0);
    int write(char *buffer, int size, int flags = 0);

    virtual ~DatagramServer();

protected:
    struct sockaddr_in serversockaddr;
    int id;
    int serverPort;
};

#endif
