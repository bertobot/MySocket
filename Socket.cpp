#include "Socket.h"
/////////////////////////////////////////////////
void Socket::pre_init(int d, int t, int p, int s) {
    domain = d;
    type = t;
    protocol = p;
    shutdown_method = s;

    _debug = 0;

    error = false;

    mClosed = false;
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
    pre_init();
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

    char *buffer = new char[size];

    bzero(buffer, size);

    read(buffer, size);

    result = buffer;
    
    return result;
}
/////////////////////////////////////////////////
int Socket::write(const char* buffer, int size, int flags)
{
    int len = ::send(socket_descriptor, buffer, size, flags);

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

int Socket::writeLine(const std::string &str, int flags) {
    std::string sstr(str + "\r\n");
    return write(sstr.c_str(), sstr.length(), flags);
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

    mClosed = true;

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
int Socket::write(const std::string &str, int flags) {
    int len = ::send(socket_descriptor, str.c_str(), str.length(), flags);
    return len;
}
/////////////////////////////////////////////////
bool Socket::isConnected() {
    int errnum;
    int errsize = sizeof(errnum);
    int rc = getsockopt(socket_descriptor, SOL_SOCKET, SO_ERROR, &errnum, (socklen_t*)&errsize);

    return errnum == 0;
}

void Socket::setSocketDescriptor(int sd) {
    socket_descriptor = sd;
}

void Socket::setError(bool err) {
    error = err;
}

int Socket::setKeepalive(bool k) {
    int keepalive;

    k ? keepalive = 1 : keepalive = 0;

    return setsockopt(socket_descriptor, SOL_SOCKET, SO_KEEPALIVE, &keepalive, sizeof(keepalive));

}

int Socket::setLinger(bool l, int timeout) {
    linger so_linger;

    l ? so_linger.l_onoff = 1 : so_linger.l_onoff = 0;

    so_linger.l_linger = timeout;

    return setsockopt(socket_descriptor, SOL_SOCKET, SO_LINGER, &so_linger, sizeof(linger));

}

bool Socket::closed() {
    return mClosed;
}

// vim: ts=4:sw=4:expandtab
