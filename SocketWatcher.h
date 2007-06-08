#ifndef __SocketWatcher_h_
#define __SocketWatcher_h_

#include "WatchedSocket.h"

/* According to POSIX.1-2001 */
#include <sys/select.h>

/* According to earlier standards */
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#include <set>
#include "../lib/MyThread/thread"

// TODO: Implement as a singleton

class SocketWatcher : publc thread  {
public:
    SocketWatcher();

    virtual void run();

    void watch(WatchedSocket &);
    bool isWatched(WatchedSocket &);

    virtual ~SocketWatcher();
private:
    void init();


protected:
    fd_set
        *read_fds,
        *write_fds,
        *error_fds;

    bool
        addingSockets,
        removingSockets;

    std::set<Socket *> socketMap;
};
#endif
