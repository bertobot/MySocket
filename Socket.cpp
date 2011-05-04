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
int Socket::get(int size) {
    char *buffer = new char[size];

	int count = 0;
	int left = 0;

    bzero(buffer, size);

	while (left < size) {
		count = ::read(socket_descriptor, buffer, (size - left) );

		if (count > 0) {
			for (int i = 0; i < count; i++) {
				_buffer += buffer[i];
			}
		}

		else
			return left;

		left += count;
	}

    return size;
}
/////////////////////////////////////////////////
int Socket::getByte() {
    char buffer[1];

    int in = ::read(socket_descriptor, &buffer, sizeof(buffer) );
    int p = (int)buffer[0];

    if (in >= 0)
        return p;

    return -1;
}
/////////////////////////////////////////////////
int Socket::write(const std::string &str) {
    int len = ::write(socket_descriptor, str.c_str(), str.length() );
    //int len = ::send(socket_descriptor, str.c_str(), str.length() );
    return len;
}
/////////////////////////////////////////////////
int Socket::write(const std::string &str, int flags) {
    //int len = ::write(socket_descriptor, str.c_str(), str.length() );
    int len = ::send(socket_descriptor, str.c_str(), str.length(), flags);
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
bool Socket::hasLine() {
    for (unsigned int i = 0; i < _buffer.length(); i++) {
        if (_buffer[i] == '\n')
            return true;
    }

    return false;
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
    std::string result;

	char *buffer = new char[length];

    int count = 0;
	int left = 0;

	while (left < length) {
    	count = ::read(socket_descriptor, buffer, (length - left) );

		if (_debug > 0)
			printf("upToLength: %d / %d\n", left, length);

		if (count < 0) {
			if (_debug > 0)
				printf("there was an error reading from socket.\n");
			break;
		}

		// copy bits over
		for (int i = 0; i < count; i++) {
			result += buffer[i];
		}

		left += count;
	}

	delete [] buffer;

    return result;
}
/////////////////////////////////////////////////
std::string Socket::upToChar(char c) {
    /*
    int endpos = _buffer.find(c) + 1;
    std::string substr = _buffer.substr(0, endpos);

    if (endpos <= 0)
        _buffer.clear();
    else
        _buffer.erase(0, endpos);

    return substr;
    */

    std::string result;

    int t = -1;

    do {
        t = getByte();

        if (t < 0)
            break;

		if (_debug > 0)
			printf ("socket read: %c, %d\n", t, t);

        result += (char)t;
        if (t == c)
            break;
    } while (t != -1);

    return result;
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
Socket::~Socket() {
// 07.12.2006 - berto
// don't auto-destruct.
//
//    close();
//    printf("[Socket] destruct.\n");
}
/////////////////////////////////////////////////
