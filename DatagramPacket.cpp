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
    return recvfrom(sock, buffer, size, flags, NULL, NULL);
}
/////////////////////////////////////////////////
int DatagramPacket::write(char* buffer, int size, int flags)
{
    return sendto(sock, buffer, size, flags, remoteaddr->ai_addr, remoteaddr->ai_addrlen);
}

int DatagramPacket::write(const std::string &message, int flags) {
    // strdup
    char *buffer = new char[message.length()];
    strncpy(buffer, message.c_str(), message.length());

    int rc = write(buffer, message.length(), flags);

    delete [] buffer;

    return rc;
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
int DatagramPacket::getSocket() {
    return sock;
}

void DatagramPacket::setSocket(int fd) {
    sock = fd;
}

struct sockaddr_in & DatagramPacket::getClientSockAddr() {
    return clientsockaddr;
}

