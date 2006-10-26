#include "Socket.h"
/////////////////////////////////////////////////
void Socket::pre_init(int d, int t, int p, int s) {
    domain = d;
    type = t;
    protocol = p;
    shutdown_method = s;
    error = false;
}
/////////////////////////////////////////////////
void Socket::init(int d, int t, int p, int s) {
    pre_init(d, t, p, s);    
    socket_descriptor = socket(d, t, p);
}
/////////////////////////////////////////////////
Socket::Socket() {
    init();
}
/////////////////////////////////////////////////
Socket::Socket(int d, int t, int p, int s) {
    init(d, t, p, s);
}
/////////////////////////////////////////////////
Socket::Socket(int fd=-1) {
    socket_descriptor = fd;
    //error = false;
    //domain = type = protocol = shutdown_method = 0;
    pre_init();
}
/////////////////////////////////////////////////
Socket::Socket(const Socket & s) {
    socket_descriptor = s.socket_descriptor;
    pre_init();
}
/////////////////////////////////////////////////
Socket Socket::operator=(const Socket &rhs) {
    // if same object, return
    if (this == &rhs)
        return *this;

    // clear?  no

    socket_descriptor = rhs.socket_descriptor;
    init();

    return *this;
}
/////////////////////////////////////////////////
void Socket::setDomain(int d) {
    domain = d;
}
/////////////////////////////////////////////////
void Socket::setType(int t) {
    type = t;
}
/////////////////////////////////////////////////
void Socket::setProtocol(int p) {
    protocol = p;
}
/////////////////////////////////////////////////
int Socket::getSocketDescriptor() {
    return socket_descriptor;
}
/////////////////////////////////////////////////
int Socket::getDomain() {
    return domain;
}
/////////////////////////////////////////////////
int Socket::getType() {
    return type;
}
/////////////////////////////////////////////////
int Socket::getProtocol() {
    return protocol;
}
/////////////////////////////////////////////////
std::string Socket::read() {
   std::string newStr;
   char buffer[1024];
   uint in = 1024;

   while (in == 1024) {
       bzero(buffer, 1024);
       in = ::read(socket_descriptor, buffer, 1024);
       std::string nstr(buffer);
       newStr += nstr;
   }

   error = (in == 0);
   return newStr;
}
/////////////////////////////////////////////////
int Socket::write(const std::string &str) {
    int len = ::write(socket_descriptor, str.c_str(), str.length() );
    //int len = ::send(socket_descriptor, str.c_str(), str.length() );
    return len;
}
/////////////////////////////////////////////////
bool Socket::isValid() {
    return (socket_descriptor >= 0) && !error;
}
/////////////////////////////////////////////////
int Socket::close() {
    int rc = shutdown(socket_descriptor, 2);
    printf("[Socket: close] shutdown.\n");

    return rc;
}
/////////////////////////////////////////////////
std::string Socket::getIP() {
    struct sockaddr_in client_sockaddr;
    int size = sizeof(client_sockaddr);

    int errorcheck = getpeername(socket_descriptor, (struct sockaddr*) &client_sockaddr, (socklen_t *)&size );
    if (errorcheck == 0)
        return inet_ntoa (client_sockaddr.sin_addr);
    return "failed";
}
/////////////////////////////////////////////////
Socket::~Socket() {
// 07.12.2006 - berto
// don't auto-destruct.
//
//    close();
//    printf("[Socket] destruct.\n");
}
/////////////////////////////////////////////////
// friend?
int socketCleanup(int fd, int how=2) {
    return shutdown(fd, how);
}
/////////////////////////////////////////////////
