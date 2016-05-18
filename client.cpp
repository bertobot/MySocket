#include "ClientSocket.h"
/////////////////////////////////////////////////
int main() {
    ClientSocket client("localhost", 33000);

    if (client.connect(5) )
        client.write("greetings from the client!");
    else 
        printf ("something went wrong: %s\n", strerror(errno));

    // clean up
    client.close();

    return 0;
}
/////////////////////////////////////////////////
