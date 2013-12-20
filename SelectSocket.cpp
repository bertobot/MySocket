#include "SelectSocket.h"

SelectSocket::SelectSocket() : Select() {

}

SelectSocket::~SelectSocket() {

}

void SelectSocket::add(const Socket &s) {
	Select::add(s.getSocketDescriptor() );
}

void SelectSocket::remove(const Socket &s) {
	Select::remove(s.getSocketDescriptor() );
}

std::vector<Socket> SelectSocket::canRead() {
	std::vector<Socket> result;

	std::vector<int> ready = Select::canRead();

	for (unsigned int i = 0; i < ready.size(); i++)
		result.push_back(Socket(ready[i]) );

	return result;
}

std::vector<Socket> SelectSocket::canWrite() {
	std::vector<Socket> result;

	std::vector<int> ready = Select::canWrite();

	for (unsigned int i = 0; i < ready.size(); i++)
		result.push_back(Socket(ready[i]) );

	return result;
}

std::vector<Socket> SelectSocket::canReadWrite() {
	std::vector<Socket> result;

	std::vector<int> ready = Select::canReadWrite();

	for (unsigned int i = 0; i < ready.size(); i++)
		result.push_back(Socket(ready[i]) );

	return result;
}

