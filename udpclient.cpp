#include "DatagramPacket.h"

int main() {
    char *buffer = new char[1024];
    char *payload = "hello";

    DatagramPacket dp("localhost", "35000");
    dp.write(payload, 5);

    dp.read(buffer, 1024);
    printf("server response: '%s'\n", buffer);

    dp.close();

    delete [] buffer;

    return 0;
}
