#include "DatagramPacket.h"
/////////////////////////////////////////////////
DatagramPacket::DatagramPacket()
{
    remoteaddr = NULL;
    addrresult = NULL;

    init();
}
/////////////////////////////////////////////////
DatagramPacket::DatagramPacket(const std::string& addr, const std::string& port)
{
    this->address = addr;
    this->port = port;
    init();
}
/////////////////////////////////////////////////
int DatagramPacket::getSocketDescriptor()
{
    return sock;
}
/////////////////////////////////////////////////
void DatagramPacket::setSocketDescriptor(int sock)
{
    this->sock = sock;
}
/////////////////////////////////////////////////
int DatagramPacket::read(char* buffer, int size, int flags)
{
    socklen_t len = sizeof(clientsockaddr);
    return recvfrom(sock, buffer, size, flags, (struct sockaddr*)&clientsockaddr, &len);
}
/////////////////////////////////////////////////
int DatagramPacket::write(char* buffer, int size, int flags)
{
    return sendto(sock, buffer, size, flags, remoteaddr->ai_addr, remoteaddr->ai_addrlen);
}
/////////////////////////////////////////////////
void DatagramPacket::init()
{
    sock = socket(AF_INET, SOCK_DGRAM, 0);

    addrinfo hints;

    // setup hints

    memset(&hints, 0, sizeof(struct addrinfo));

    hints.ai_family = AF_UNSPEC;        /* Allow IPv4 or IPv6 */
    hints.ai_socktype = SOCK_DGRAM;   /* Datagram socket */    
    hints.ai_flags = 0;
    hints.ai_protocol = 0;              /* Any protocol */

    // get info.

    int rc = getaddrinfo(address.c_str(), port.c_str(), &hints, &addrresult);

    if (rc == 0) {
	remoteaddr = addrresult;
    }
}
/////////////////////////////////////////////////
void DatagramPacket::clear()
{
    // clear result
    freeaddrinfo(addrresult);
}
/////////////////////////////////////////////////
void DatagramPacket::close()
{
    ::close(sock);
}
/////////////////////////////////////////////////
DatagramPacket::~DatagramPacket()
{
    clear();
}
/////////////////////////////////////////////////