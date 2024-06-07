#include "stuff.hpp"
#include "utils.hpp"

int main() {
	cout << "-------------------" << endl;
	cout << "AUTOINDEX" << endl;
	cout << "-------------------" << endl;

    int serverFd, newSocket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    if ((serverFd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        cerr << "Socket failed" << endl;
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(serverFd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        cerr << "Bind failed" << endl;
        close(serverFd);
        exit(EXIT_FAILURE);
    }

    if (listen(serverFd, 3) < 0) {
        cerr << "Listen failed" << endl;
        close(serverFd);
        exit(EXIT_FAILURE);
    }

    while (true) {
        if ((newSocket = accept(serverFd, (struct sockaddr*)&address, (socklen_t*)&addrlen)) < 0) {
            cerr << "Accept failed" << endl;
            close(serverFd);
            exit(EXIT_FAILURE);
        }
        handleClient(newSocket);
    }

    close(serverFd);
    return 0;
}
