#include "DatagramServer.h"

int main() {
    char *buffer = new char[1024];

    char *payload = "bye!";

    DatagramServer ds(35000);

    if (ds.isBound() ) {	
	ds.read(buffer, 1024);
	printf("server received: '%s'\n", buffer);

	ds.write(payload, 4);
    }

    ds.close();

    delete [] buffer;

    return 0;
}
