#ifndef __DatagramServer_h_
#define __DatagramServer_h_

#include "DatagramPacket.h"

class DatagramServer : public DatagramPacket {
public:
    DatagramServer(int port);

    bool isBound();
    void init();

    virtual ~DatagramServer();

protected:
    struct sockaddr_in serversockaddr;
    int id;
    int serverPort;
};

#endif
