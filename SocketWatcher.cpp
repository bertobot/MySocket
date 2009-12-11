#include "SocketWatcher.h"
/////////////////////////////////////////////////
SocketWatcher::SocketWatcher() : thread() {
    done = false;

    // initialize timeout
    ts.tv_sec = 0;
    ts.tv_nsec = 1000000;   // default to 1 milliseconds 1 000 000

    init();
}
/////////////////////////////////////////////////
void SocketWatcher::watch(WatchedSocket *&ws) {
    if (ws) {
        socketMapMutex.lock();

        addToFDSet(ws->getSocketDescriptor() );
        socketMap.insert(ws);

        socketMapMutex.unlock();
    }
}
/////////////////////////////////////////////////
void SocketWatcher::unwatch(WatchedSocket *&ws) {
    if (ws) {
        socketMapMutex.lock();

        removeFromFDSet(ws->getSocketDescriptor() );

        std::set<WatchedSocket *>::iterator sitr = socketMap.find(ws);
        socketMap.erase(sitr);

        socketMapMutex.unlock();
    }
}
/////////////////////////////////////////////////
bool SocketWatcher::isWatched(WatchedSocket *&ws) {
    bool t;
    socketMapMutex.lock();

    std::set<WatchedSocket *>::iterator sitr = socketMap.find(ws);
    t = (sitr != socketMap.end() );

    socketMapMutex.unlock();

    return t;
}
/////////////////////////////////////////////////
void SocketWatcher::addToFDSet(int fd) {
    FD_SET(fd, &read_fds);
    FD_SET(fd, &write_fds);
    FD_SET(fd, &error_fds);
}
/////////////////////////////////////////////////
void SocketWatcher::removeFromFDSet(int fd) {
    FD_CLR(fd, &read_fds);
    FD_CLR(fd, &write_fds);
    FD_CLR(fd, &error_fds);
}
/////////////////////////////////////////////////
void SocketWatcher::init() {
    // initialize sets
    FD_ZERO(&read_fds);
    FD_ZERO(&write_fds);
    FD_ZERO(&error_fds);
}
/////////////////////////////////////////////////
// signalSockets
//
// this function has to signal the sockets whether
// they can read or write.
//
// what we can write, the client socket can read
// and vice versa.
//
void SocketWatcher::signalSockets() {
    socketMapMutex.lock();
    std::set<WatchedSocket *>::iterator sitr = socketMap.begin();
    for (; sitr != socketMap.end(); sitr++) {
        // TODO: check for dead sockets here?
        int d = (*sitr)->getSocketDescriptor();
        if (d > 0) {
            // remember, what we see as 'read', is a 'write' from the client sockets.
            if (FD_ISSET(d, &read_fds) )
                (*sitr)->setWrite();

            if (FD_ISSET(d, &write_fds) )
                (*sitr)->setRead();

            if (FD_ISSET(d, &error_fds) )
                (*sitr)->setEventError();
        }

        else {
            std::cout << "[SocketWatcher: signalSockets] socket dead." << std::endl;
            socketMap.erase(sitr);
        }
    }
    socketMapMutex.unlock();
}
/////////////////////////////////////////////////
void SocketWatcher::run() {

    while (!done) {
        // run pselect with timeout.
        // after pselect times out, check for events (fd_isset).
        //      if fd_isset, update WatchedSocket's event state.
        // check if removed sockets
        //      remove sockets from set atomically!
        // check if added new sockets
        //      add sockets to set atomically!


        // set read fds to the list of file descriptors (fd's)
        // from the map that we have

        int nfds = getHighestFD() + 1;
        int ready = pselect(nfds, &read_fds, &write_fds, &error_fds, &ts, NULL);

        // signal watched sockets on event
        if (ready > -1)
            signalSockets();

        printf("pselect returned %d\n", ready);

        // loop!
    }
}
/////////////////////////////////////////////////
void SocketWatcher::setTimeout(long sec, long nano) {
    ts.tv_sec = sec;
    ts.tv_nsec = nano;
}
/////////////////////////////////////////////////
void SocketWatcher::stop() {
    done = true;
}
/////////////////////////////////////////////////
void SocketWatcher::clear() {
    socketMapMutex.lock();
    init();     // clear the FD SETS
    socketMap.clear();
    socketMapMutex.unlock();
}
/////////////////////////////////////////////////
long SocketWatcher::getHighestFD() {
    long max = 0;
    socketMapMutex.lock();
    std::set<WatchedSocket *>::iterator sitr = socketMap.begin();
    for (; sitr != socketMap.end(); sitr++) {
        long d = (*sitr)->getSocketDescriptor();
        if (d > max)
            max = d;
    }
    socketMapMutex.unlock();

    return max;
}
/////////////////////////////////////////////////
SocketWatcher::~SocketWatcher() {
    stop();
}
/////////////////////////////////////////////////