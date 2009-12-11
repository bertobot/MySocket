#include "WatchedSocket.h"
/////////////////////////////////////////////////
WatchedSocket::WatchedSocket() : Socket() {
    clearWatch();

    // set default to 1 millisecond 1 000 000
    setTimeout(0, 1000000);
}
/////////////////////////////////////////////////
WatchedSocket::WatchedSocket(const Socket& rhs) : Socket(rhs) {
    clearWatch();

    // set default to 1 millisecond 1 000 000
    setTimeout(0, 1000000);
}
/////////////////////////////////////////////////
bool WatchedSocket::canRead() {
    bool t = flagRead;
    flagRead = false;

    return t;
}
/////////////////////////////////////////////////
bool WatchedSocket::canWrite() {
    bool t = flagWrite;
    flagWrite = false;

    return t;
}
/////////////////////////////////////////////////
bool WatchedSocket::hasEventError() {
    bool t = flagEventError;
    flagEventError = false;

    return t;
}
/////////////////////////////////////////////////
void WatchedSocket::setRead() {
    flagRead = true;
}
/////////////////////////////////////////////////
void WatchedSocket::setWrite() {
    flagWrite = true;
}
/////////////////////////////////////////////////
void WatchedSocket::setEventError() {
    flagEventError = true;
}
/////////////////////////////////////////////////
void WatchedSocket::clearWatch() {
    flagRead = false;
    flagWrite = false;
    flagEventError = false;
}
/////////////////////////////////////////////////
bool WatchedSocket::waitForRead() {
    int nfds = 0;
    int result = pselect(nfds, NULL, NULL, NULL, &ts, NULL);
    return canRead();
}
/////////////////////////////////////////////////
void WatchedSocket::setTimeout(long s, long ns) {
    ts.tv_sec = s;
    ts.tv_nsec = ns;
}
/////////////////////////////////////////////////
WatchedSocket::~WatchedSocket() {

}
/////////////////////////////////////////////////
