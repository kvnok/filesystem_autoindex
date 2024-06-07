#include "stuff.hpp"
#include "utils.hpp"

#include <filesystem>

namespace fs = filesystem;

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
    
    string header = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
    string body = generateDirectoryListing(".");
    string response = header + body;

    send(clientSocket, response.c_str(), response.length(), 0);
    close(clientSocket);
}
