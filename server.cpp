#include "ServerSocket.h"
#include <iostream>
/////////////////////////////////////////////////
int main() {
    ServerSocket server(33000);

    if (!server.isValid() ) {
        printf("binding error\n");
        return 1;
    }

    if (server.listen(5) ) {
        Socket s(server.accept() );
        std::string str = s.read();
        std::cout << "[server] message length: " << str.length() << std::endl;
        std::cout << "[server] '" << str << "'" << std::endl;
    }

    else
        printf ("couldn't listen\n");

    return 0;
}
/////////////////////////////////////////////////
