#include "Socket.h"
/////////////////////////////////////////////////
void Socket::pre_init(int d, int t, int p, int s) {
    domain = d;
    type = t;
    protocol = p;
    shutdown_method = s;

    _debug = 0;

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
Socket::Socket(int t, int d, int p, int s) {
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
int Socket::getSocketDescriptor() const {
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
/**
 * get(char * buffer, int size)
 *
 * this method fetches data of 'size' length, and stores it
 * the provided char buffer.
 */
int Socket::read(char * buffer, int size) {

    if (! buffer)
		throw NIOException("first parameter is null.");


    int count = ::read(socket_descriptor, buffer, size);

	if (count < 0)
		throw NIOException("Read Fail.", count);

    return count;
}
/////////////////////////////////////////////////
/**
 * get(char * buffer, int size)
 *
 * this method fetches data of 'size' length, and stores it
 * the provided char buffer.
 */
int Socket::read(char * buffer, int size, int flags) {

    if (! buffer)
		throw NIOException("first paramater is null.");

    int count = ::recv(socket_descriptor, buffer, size, flags);

	if (count < 0)
		throw NIOException("Read Fail.", count);

    return count;
}
/////////////////////////////////////////////////
char Socket::readByte() {
    char buffer[1];

    int in = ::read(socket_descriptor, &buffer, sizeof(buffer) );
    
    if (in > 0)
        return buffer[0];

	throw NIOException("Read Fail.", -1);
}
/////////////////////////////////////////////////
char Socket::readByte(int flags) {
    char buffer[1];

    int in = ::recv(socket_descriptor, &buffer, sizeof(buffer), flags);

    if (in >= 0)
        return buffer[0];

	throw NIOException("Read Fail.", -1);
}
/////////////////////////////////////////////////
std::string Socket::read(int size)
{
    std::string result;

    try {
        for (int i = 0; i < size; i++) {
            char c = readByte();
            
            result += (char)c;
        }
    }
    catch(NIOException n) {
        // do nothing
    }
    
    return result;
}
/////////////////////////////////////////////////
int Socket::write(char* buffer, int size)
{
    int len = ::write(socket_descriptor, buffer, size);

    if (len < 0)
        throw NIOException("Write Fail.", len);

    return len;
}
/////////////////////////////////////////////////
int Socket::write(char* buffer, int size, int flags)
{
    int len = ::send(socket_descriptor, buffer, size, flags);

    if (len < 0)
        throw NIOException("Write Fail.", len);

    return len;
}
/////////////////////////////////////////////////
int Socket::writeByte(char c)
{
    int len = ::write(socket_descriptor, &c, 1);

    if (len < 0)
        throw NIOException("Write Fail.", len);

    return len;
    
}
/////////////////////////////////////////////////
int Socket::writeByte(char c, int flags)
{
    int len = ::send(socket_descriptor, &c, 1, flags);

    if (len < 0)
        throw NIOException("Write Fail.", len);

    return len;
}
/////////////////////////////////////////////////
bool Socket::isValid() {
    return (socket_descriptor >= 0) && !error;
}
/////////////////////////////////////////////////
int Socket::close() {
    if (_debug > 0)
	printf("[Socket: close] %d\n", socket_descriptor);

    int rc = ::shutdown(socket_descriptor, 2);
    rc = ::close(socket_descriptor);

    socket_descriptor = -1;
    return rc;
}
/////////////////////////////////////////////////
std::string Socket::getClientIP() {
    struct sockaddr_in client_sockaddr;
    int size = sizeof(client_sockaddr);

    int errorcheck = getpeername(socket_descriptor, (struct sockaddr*) &client_sockaddr, (socklen_t *)&size );
    if (errorcheck == 0)
        return inet_ntoa (client_sockaddr.sin_addr);
    return "failed";
}
/////////////////////////////////////////////////
long Socket::getClientPort() {
    struct sockaddr_in client_sockaddr;
    int size = sizeof(client_sockaddr);

    int errorcheck = getsockname(socket_descriptor, (struct sockaddr*) &client_sockaddr, (socklen_t *) &size);

    if (errorcheck == 0)
        return client_sockaddr.sin_port;

    return 0;
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
    return setsockopt(socket_descriptor, level, optname, &boolean, sizeof(int) );
}
/////////////////////////////////////////////////
void Socket::setDebug(int d) {
	_debug = d;
}
/////////////////////////////////////////////////
int Socket::getDebug() {
	return _debug;
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
    std::string result;

    int t = -1;

    do {
        t = readByte();

        if (_debug > 0)
            printf ("socket read: %c, %d\n", t, t);

        if (t == '\n')
            break;

        result += (char)t;

    } while (t != -1);

    int len = result.size() - 1;

    if (result[len] == '\r')
        result = result.substr(0, len);

    return result;
}
/////////////////////////////////////////////////
int Socket::write(const std::string &str) {
    int len = ::write(socket_descriptor, str.c_str(), str.length() );
    //int len = ::send(socket_descriptor, str.c_str(), str.length() );

	if (len < 0)
		throw NIOException("Write failed.", len);
	
    return len;
}
/////////////////////////////////////////////////
int Socket::write(const std::string &str, int flags) {
    //int len = ::write(socket_descriptor, str.c_str(), str.length() );
    int len = ::send(socket_descriptor, str.c_str(), str.length(), flags);
    return len;
}
/////////////////////////////////////////////////
int Socket::writeLine(const std::string &str, int flags) {
    std::string package = str + "\r\n";
    int len = ::send(socket_descriptor, package.c_str(), package.length(), flags);

	if (len < 0)
		throw NIOException("Write failed.", len);
	
    return len;
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

// vim: ts=4:sw=4:expandtab
