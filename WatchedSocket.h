#ifndef __WatchedSocket_h_
#define __WatchedSocket_h_

#include "Socket.h"

class WatchedSocket : public Socket {
public:
    WatchedSocket();

    bool canRead();
    bool canWrite();
    bool hasEventError();

    virtual ~WatchedSocket();

private:

protected:
    bit
        read,
        write,
        eventError;
};
#endif
