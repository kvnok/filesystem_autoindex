#include "stuff.hpp"
#include "utils.hpp"

int main() {
	print(CYN, "-------------------");
	print(CYN, "AUTOINDEX");
	print(CYN, "-------------------");

    int serverFd, newSocket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    if ((serverFd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        print(RED, "Socket failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(serverFd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        print(RED, "Bind failed");
        close(serverFd);
        exit(EXIT_FAILURE);
    }

    if (listen(serverFd, 3) < 0) {
        print(RED, "Listen failed");
        close(serverFd);
        exit(EXIT_FAILURE);
    }
	print(YEL, "Server listening on port ", PORT);
    while (true) {
		print(YEL, "Waiting for connection...");
        if ((newSocket = accept(serverFd, (struct sockaddr*)&address, (socklen_t*)&addrlen)) < 0) {
            print(RED, "Accept failed");
            close(serverFd);
            exit(EXIT_FAILURE);
        }
		print(GRN, "Connection accepted");
        handleClient(newSocket);
		print(GRN, "Connection closed");
    }

    close(serverFd);
    return 0;
}
