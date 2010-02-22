#ifndef __Select_h_
#define __Select_h_

#include <vector>

#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>

class Select {
public:
    Select();
    Select(const std::vector<int>&);

    int canRead();
    int canWrite();
    bool hasEventError();

    void add(int);
    void remove(int);
    void clear();

    void wait();

    void setTimeout(long sec, long nsec);

    virtual ~Select();

private:
    void findMax();
    void add_fd(int);
    void remove_fd(int);
    void prepare();
    void init();

protected:
    fd_set
        read_fds,
        write_fds,
        error_fds;

    struct timespec ts;

    std::vector<int> fds;
    int max;
};
#endif
