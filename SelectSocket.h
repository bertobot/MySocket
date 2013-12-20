#ifndef __SelectSocket_h_
#define __SelectSocket_h_

#include "Select.h"
#include "Socket.h"

class SelectSocket : public Select {
public:
    SelectSocket();
    virtual ~SelectSocket();

    std::vector<Socket> canRead();
	std::vector<Socket> canWrite();
	std::vector<Socket> canReadWrite();

	void add(const Socket &s);
	void remove(const Socket &s);

};

#endif
// vim: set ts=4:sw=4:expandtab
