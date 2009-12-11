#ifndef __WatchedSocket_h_
#define __WatchedSocket_h_

#include <MySocket/Socket.h>
#include <MyThread/mutex.h>

#include <errno.h>

class WatchedSocket : public Socket {
public:
    WatchedSocket();
    WatchedSocket(const Socket&);

    bool canRead();
    bool canWrite();
    bool hasEventError();

    void setRead();
    void setWrite();
    void setEventError();

    bool waitForRead();
    bool waitForWrite();

    void clearWatch();

    void setTimeout(long sec, long nsec);

    virtual ~WatchedSocket();

private:

protected:
    struct timespec ts;

    bool
        flagRead,
        flagWrite,
        flagEventError;
};
#endif
