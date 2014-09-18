#include "BufferedReader.h"

BufferedReader::BufferedReader(Socket *socket) {
	mSocket = socket;
}

BufferedReader::~BufferedReader() {
	mSocket = NULL;
}

BufferedReader::operator bool() const {
	return ! empty();
}

std::string BufferedReader::readLine(int rsize) {
	
	if (! mSocket)
		throw NIOException("associated socket is NULL.");

	std::string result;

	size_t mpos = mBuffer.find('\n');

	int rc = 0;

	while (mpos == std::string::npos) {
		char *buffer = new char[rsize];

		bzero(buffer, rsize);

		rc = mSocket->read(buffer, rsize - 1);

		mBuffer += buffer;

		delete [] buffer;

		buffer = NULL;

		mpos = mBuffer.find('\n');

		if (rc == 0) {
			result = mBuffer;
			mBuffer = "";
			return result;
		}

		if (rc < 0)
			throw NIOException("readLine failed.");
	}

	result = mBuffer.substr(0, mpos);

	if (result[mpos - 1] == '\r')
		result = result.substr(0, mpos - 1);

	mBuffer = mBuffer.substr(mpos + 1);

	return result;
}

std::string BufferedReader::read(int size) {
	std::string result;

	if (mBuffer.size() <= size) {
		result = mBuffer.substr(0, size);

		if (size + 1 > mBuffer.size() )
			mBuffer = "";
		else
			mBuffer = mBuffer.substr(size+1);
	}

	else {
		int readSize = size - mBuffer.size();

		char *buffer = new char[readSize];

		if (! mSocket)
			throw NIOException("socket is null");

		mSocket->read(buffer, readSize);

		result = mBuffer + buffer;

		mBuffer = "";

		delete [] buffer;
		buffer = NULL;
	}

	return result;
}

bool BufferedReader::empty() const {
	return mBuffer.empty();
}

