#include "Select.h"
/////////////////////////////////////////////////
Select::Select() {
    init();

    // set default to 1 millisecond 1 000 000
    setTimeout(0, 1000000);
    max = 0;
}
/////////////////////////////////////////////////
Select::Select(const std::vector<int> &list) {
    init();
    setTimeout(0, 1000000);
    max = 0;

    for (int i = 0; i < list.size(); i++) {
        add(list[i]);
    }
}
/////////////////////////////////////////////////
void Select::add(int fd) {
    fds.push_back(fd);
    if (fd > max)
        max = fd;
}
/////////////////////////////////////////////////
void Select::remove(int fd) {
    std::vector<int>::iterator itr = fds.begin();
    while (itr != fds.end() ) {
        if ( (*itr) == fd) {
            fds.erase(itr);
            remove_fd(fd);
            itr = fds.begin();
        }
        else {
            itr++;
        }
    }

    findMax();
}
/////////////////////////////////////////////////
void Select::findMax() {
    max = 0;
    std::vector<int>::iterator itr = fds.begin();
    for (; itr != fds.end(); itr++) {
        if (*itr > max)
            max = *itr;
    }
}
/////////////////////////////////////////////////
void Select::add_fd(int fd) {
    FD_SET(fd, &read_fds);
    FD_SET(fd, &write_fds);
    FD_SET(fd, &error_fds);
}
/////////////////////////////////////////////////
void Select::remove_fd(int fd) {
    FD_CLR(fd, &read_fds);
    FD_CLR(fd, &write_fds);
    FD_CLR(fd, &error_fds);
}
/////////////////////////////////////////////////
void Select::init() {
    // initialize sets
    FD_ZERO(&read_fds);
    FD_ZERO(&write_fds);
    FD_ZERO(&error_fds);
}
/////////////////////////////////////////////////
void Select::prepare() {
	init();

    std::vector<int>::iterator itr = fds.begin();
    for (; itr != fds.end(); itr++) {
        add_fd(*itr);
    }
}
/////////////////////////////////////////////////
void Select::clear() {
    init();
}
/////////////////////////////////////////////////
std::vector<int> Select::canRead() {
    std::vector<int> result;
    struct timespec lts;

    lts.tv_sec = ts.tv_sec;
    lts.tv_nsec = ts.tv_nsec;

    prepare();

    int nfds = max + 1;
    //int ready = pselect(nfds, &read_fds, &write_fds, &error_fds, &lts, NULL);
    int ready = pselect(nfds, &read_fds, NULL, NULL, &lts, NULL);

    //printf("ready: %d\n", ready);

    if (ready == -1) {
        // TODO: EXCEPTION
        printf("exception in Select: %s\n", strerror(errno) );
        return result;
    }

    for (int i = 0; i < nfds; i++) {
        if (FD_ISSET(i, &read_fds) ) {
            result.push_back(i);
        }
    }

    return result;
}
/////////////////////////////////////////////////
std::vector<int> Select::canWrite() {
    std::vector<int> result;
    struct timespec lts;

    lts.tv_sec = ts.tv_sec;
    lts.tv_nsec = ts.tv_nsec;

    prepare();

    int nfds = max + 1;
    int ready = pselect(nfds, NULL, &write_fds, NULL, &lts, NULL);

    if (ready == -1) {
        // TODO: EXCEPTION
        printf("exception in Select: %s\n", strerror(errno) );
        return result;
    }

    for (int i = 0; i < nfds; i++) {
        if (FD_ISSET(i, &write_fds) )
            result.push_back(i);
    }

    return result;
}
/////////////////////////////////////////////////
std::vector<int> Select::canReadWrite() {
    std::vector<int> result;
    struct timespec lts;

    lts.tv_sec = ts.tv_sec;
    lts.tv_nsec = ts.tv_nsec;

    prepare();

    int nfds = max + 1;
    int ready = pselect(nfds, &read_fds, &write_fds, NULL, &lts, NULL);

    if (ready == -1) {
        // TODO: EXCEPTION
        printf("exception in Select: %s\n", strerror(errno) );
        return result;
    }

    for (int i = 0; i < nfds; i++) {
        if (FD_ISSET(i, &read_fds) )
            result.push_back(i);

		else if (FD_ISSET(i, &write_fds) )
			result.push_back(i);
    }

    return result;
}
/////////////////////////////////////////////////
void Select::wait() {
    int nfds = 0;
    struct timespec lts;

    lts.tv_sec = ts.tv_sec;
    lts.tv_nsec = ts.tv_nsec;

    pselect(nfds, NULL, NULL, NULL, &lts, NULL);
}
/////////////////////////////////////////////////
void Select::setTimeout(long s, long ns) {
    ts.tv_sec = s;
    ts.tv_nsec = ns;
}
/////////////////////////////////////////////////
void Select::setTimeout(double t) {
    // first, get the base
    long t1 = t / 1;
    double t2 = (t / 1.0) - t1;

    // 1 billion nanos = 1 second
    long t3 = t2 * 1000000000;

    setTimeout(t1, t3);
}
/////////////////////////////////////////////////
Select::~Select() {

}
/////////////////////////////////////////////////
