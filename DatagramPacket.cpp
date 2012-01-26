#include "DatagramPacket.h"
/////////////////////////////////////////////////
DatagramPacket::DatagramPacket()
{
    sock = -1;
    remoteaddr = NULL;
    addrresult = NULL;
}
/////////////////////////////////////////////////
DatagramPacket::DatagramPacket(const std::string& addr, const std::string& port)
{
    this->address = addr;
    this->port = port;
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
    return recvfrom(sock, buffer, size, flags, remoteaddr->ai_addr, &remoteaddr->ai_addrlen);
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
DatagramPacket::~DatagramPacket()
{
    clear();
}
/////////////////////////////////////////////////
