#ifndef __BufferedReader_h_
#define __BufferedReader_h_

#include "Socket.h"

#include "NIOException.h"

#include <string>
#include <sstream>

class BufferedReader {
public:
	BufferedReader(Socket *socket);

	virtual ~BufferedReader();

	operator bool() const;

	std::string readLine(int rsize=8192);

	std::string read(int size);

	bool empty() const;

private:
	std::string mBuffer;
	Socket *mSocket;
};

#endif

