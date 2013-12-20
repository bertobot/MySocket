#ifndef __Select_h_
#define __Select_h_

#include "NIOException.h"

#include <vector>

#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>

class Select {
public:
    Select();
    Select(const std::vector<int>&);
    virtual ~Select();

    std::vector<int> canRead();
    std::vector<int> canWrite();
    std::vector<int> canReadWrite();
    
    bool hasEventError();

    void add(int);
    void remove(int);
    void clear();

    void wait();

    void setTimeout(long sec, long nsec);
    void setTimeout(double t);

    bool empty();

private:
    void findMax();
    void add_fd(int);
    void remove_fd(int);
    void prepare();
    void init();

    fd_set
        read_fds,
        write_fds,
        error_fds;

    struct timespec ts;

    std::vector<int> fds;
    int max;
};
#endif
// vim: ts=4:sw=4:expandtab

