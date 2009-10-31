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
void Socket::get() {
   char buffer[1024];
   uint in = 1024;

   while (in == 1024) {
       bzero(buffer, 1024);
       in = ::read(socket_descriptor, buffer, sizeof(buffer) );
       std::string nstr(buffer);

       _buffer += nstr;
   }

   error = (in == 0);
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
    int rc = ::shutdown(socket_descriptor, 2);
    rc |= ::close(socket_descriptor);

    printf("[Socket: close] %d, rc: %d\n", socket_descriptor, rc);
    socket_descriptor = -1;
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
int Socket::makeNonBlocking() {
    long currentFlags = fcntl(socket_descriptor, F_GETFL);
    currentFlags |= O_NONBLOCK;
    int rc = fcntl(socket_descriptor, F_SETFL, currentFlags);
    return rc;
}
/////////////////////////////////////////////////
int Socket::setOption(int level, int optname, int boolean) {
    // this, as of right now, is just a set-reuse-addr option
    return setsockopt(socket_descriptor, level, optname, &boolean, 1);
}
/////////////////////////////////////////////////
bool Socket::hasBuffer() {
    return ! _buffer.empty();
}
/////////////////////////////////////////////////
void Socket::clearBuffer() {
    _buffer.clear();
}
/////////////////////////////////////////////////
std::string Socket::getBuffer() {
    return _buffer;
}
/////////////////////////////////////////////////
std::string Socket::read() {
    std::string result = _buffer;
    _buffer.clear();
    return result;
}
/////////////////////////////////////////////////
// readLine
//
// override socket's read to read from the string
// buffer instead.  This function will return
// a string terminated by a newline.  the newline
// will not be part of the returned string.
//
std::string Socket::readLine() {
    return upToNewline();
}
/////////////////////////////////////////////////
// read(int length)
//
// same as read above, except instead of stopping
// at newline, stops at length.
//
std::string Socket::read(int length) {
    return upToLength(length);
}
/////////////////////////////////////////////////
// read(int length)
//
// same as read above, except instead of stopping
// at newline, stops at length.
//
std::string Socket::read(char c) {
    return upToChar(c);
}
/////////////////////////////////////////////////
std::string Socket::upToNewline() {
    return upToChar('\n');
}
/////////////////////////////////////////////////
std::string Socket::upToLength(int length) {
    std::string substr = _buffer.substr(0, length);
    _buffer.erase(0, length);

    return substr;
}
/////////////////////////////////////////////////
std::string Socket::upToChar(char c) {
    int endpos = _buffer.find(c);
    std::string substr = _buffer.substr(0, endpos);
    _buffer.erase(0, endpos);

    return substr;
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
