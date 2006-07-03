#include "ClientSocket.h"
/////////////////////////////////////////////////
int main() {
    ClientSocket client("localhost", 33000);

    client.connect();

    if (client.isConnected() ) {
        client.write("greetings from the client!");
    }
    else {
        printf ("something went wrong...\n");
    }

    return 0;
}
/////////////////////////////////////////////////
