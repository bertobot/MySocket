#include "DatagramServer.h"

DatagramServer::DatagramServer(int port) : DatagramPacket()
{
    this->serverPort = port;
    this->id = -1;

    init();
}

void DatagramServer::init()
{
    sock = socket(AF_INET, SOCK_DGRAM, 0);

    bzero(&serversockaddr,sizeof(serversockaddr));

    serversockaddr.sin_family = AF_INET;
    serversockaddr.sin_addr.s_addr=htonl(INADDR_ANY);
    serversockaddr.sin_port=htons(serverPort);

    // set to always reuse addr.
    int boolean = 1;

    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &boolean, sizeof(int) ) == 0)
	id = bind(sock, (struct sockaddr *)&serversockaddr, sizeof(serversockaddr) );
    else
	id = -1;
}

bool DatagramServer::isBound()
{
    return (id == 0);
}

DatagramServer::~DatagramServer()
{

}
