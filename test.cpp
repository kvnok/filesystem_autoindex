#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <filesystem>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

using namespace std;

namespace fs = filesystem;

const int PORT = 8080;

string generateDirectoryListing(const string& path) {
    ostringstream oss;
    oss << "<html><head><title>Index of " << path << "</title></head><body>";
    oss << "<h1>Index of " << path << "</h1><ul>";

    for (const auto& entry : fs::directory_iterator(path)) {
        oss << "<li><a href=\"" << entry.path().filename().string() << "\">" 
            << entry.path().filename().string() << "</a></li>";
    }

    oss << "</ul></body></html>";
    return oss.str();
}

void handleClient(int clientSocket) {
    char buffer[1024];
    read(clientSocket, buffer, sizeof(buffer));
    
	cout << "buffer: |" << buffer << "|" << endl;

    string header = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
    string body = generateDirectoryListing(".");
    string response = header + body;

    send(clientSocket, response.c_str(), response.length(), 0);
    close(clientSocket);
}

int main() {
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
