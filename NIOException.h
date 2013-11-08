#ifndef __NIOException_h_
#define __NIOException_h_

#include "Exception.h"

class NIOException : Exception {
private:
    int return_code;

public:
    NIOException(const std::string &m = "Generic NIOException.", int rc=0) : Exception(m) {
        return_code = rc;
    }

    virtual ~NIOException() throw() {}

    int getReturnCode() {
        return return_code;
    }

    void setReturnCode(int rc) {
        return_code = rc;
    }
};

#endif

// vim: ts=4:sw=4:expandtab
