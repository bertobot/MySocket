#ifndef __SocketWatcher_h_
#define __SocketWatcher_h_

#include "WatchedSocket.h"

/* According to POSIX.1-2001 */
#include <sys/select.h>

/* According to earlier standards */
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>

#include <set>
#include <vector>
#include <iostream>
#include <MyThread/thread.h>
#include <MyThread/mutex.h>

// TODO: Implement as a singleton

class SocketWatcher : public thread  {
public:
    SocketWatcher();

    virtual void run();

    void watch(WatchedSocket *&);
    void unwatch(WatchedSocket *&);
    bool isWatched(WatchedSocket *&);

    void setTimeout(long, long);
    void stop();

    void clear();

    virtual ~SocketWatcher();

private:
    void init();

    void addToFDSet(int);
    void removeFromFDSet(int);
    void signalSockets();

    long getHighestFD();

protected:
    fd_set
        read_fds,
        write_fds,
        error_fds;

    struct timespec ts;

    bool done;

    std::set<WatchedSocket *> socketMap;

    mutex socketMapMutex;
};
#endif
