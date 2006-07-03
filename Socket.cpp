#include "Socket.h"
/////////////////////////////////////////////////
void Socket::init(int d, int t, int p, int s) {
    domain = d;
    type = t;
    protocol = p;
    shutdown_method = s;

    socket_descriptor = socket(d, t, p);

    error = false;
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
    error = false;
}
/////////////////////////////////////////////////
Socket::Socket(const Socket *&s) {
    if (s) {
        socket_descriptor = s->socket_descriptor;
        error = false;
    }
    else {
        init();
    }
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
void Socket::close() {
    shutdown(socket_descriptor, 2);
    printf("[Socket: close] shutdown.\n");
}
/////////////////////////////////////////////////
Socket::~Socket() {
    close();
    printf("[Socket] destruct.\n");
}
/////////////////////////////////////////////////
