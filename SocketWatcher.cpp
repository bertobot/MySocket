#include "SocketWatcher.h"
/////////////////////////////////////////////////
SocketWatcher::SocketWather() : thread() {
    addingSockets = false;
    removingSockets = false;
}
/////////////////////////////////////////////////
void SocketWatcher::addWatchedSocket(WatchedSocket *&ws) {
    if (ws) {
        socketMap.insert(ws);
    }
}
/////////////////////////////////////////////////
void SocketWatcher::init() {

    int nfds;
    int result = 0;

    // initialize sets
    FD_ZERO(&read_fds);
    FD_ZERO(&write_fds);
    FD_ZERO(&error_fds);
}
/////////////////////////////////////////////////

// set read fds to the list of file descriptors (fd's)
    // from the map that we have

    std::set<Socket *>::Iterator socketMapIterator = socketMap.begin();
    while (!socketMapIterator.end() ) {
        FD_SET( (*socketMapIterator)->getSocketDescriptor(), &read_fds);
        FD_SET( (*socketMapIterator)->getSocketDescriptor(), &write_fds);
        FD_SET( (*socketMapIterator)->getSocketDescriptor(), &error_fds);
    }

    int fds = pselect(nfds, &read_fds, &write_fds, &error_fds, NULL);
    std::cout << "[debug] whileReadEvent: " << fds << std::endl;

    if (FD_ISSET(getSocketDescriptor(), &exceptionfds) ) {
        std::cout << "there are errors in the socket descriptor" << std::endl;
    }

    if (FD_ISSET(getSocketDescriptor(), &readfds) ) {
        result += 1;
    }

    if (FD_ISSET(getSocketDescriptor(), &writefds) ) {
        result += 2;
    }

    return result;
/////////////////////////////////////////////////
void SocketWatcher::run() {

    while (!done) {
        // run pselect with timeout.
        // after pselect times out, check for events (fd_isset).
        //      if fd_isset, update WatchedSocket's event state.
        // check if added new sockets
        //      remove sockets from set atomically!
        // check if removed sockets
        //      remove sockets from set atomically!
    }
}
/////////////////////////////////////////////////
SocketWatcher::~SocketWatcher() {

}
/////////////////////////////////////////////////
