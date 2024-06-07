#include "stuff.hpp"
#include "utils.hpp"

#include <filesystem>

namespace fs = filesystem;

string generateDirectoryListing(const string& path) {
    ostringstream oss;
    oss << "<html><head><title>Index of " << path << "</title></head><body>";
    oss << "<h1>Index of " << path << "</h1>";
    oss << "<table border=\"1\"><tr><th>Name</th><th>Last Modified</th><th>Size (bytes)</th></tr>";

    for (const auto& entry : fs::directory_iterator(path)) {
        if (fs::is_regular_file(entry)) {
            // Get filename
            string filename = entry.path().filename().string();

            // Get last modified time
            auto ftime = fs::last_write_time(entry);
            auto sctp = chrono::time_point_cast<chrono::system_clock::duration>(
                            ftime - decltype(ftime)::clock::now() + chrono::system_clock::now()
                        );
            time_t cftime = chrono::system_clock::to_time_t(sctp);
            char buffer[20];
            strftime(buffer, sizeof(buffer), "%d-%b-%Y %H:%M", localtime(&cftime));

            // Get file size
            uintmax_t filesize = fs::file_size(entry);

            oss << "<tr><td><a href=\"" << filename << "\">" << filename << "</a></td>";
            oss << "<td>" << buffer << "</td>";
            oss << "<td>" << filesize << "</td></tr>";
        }
    }

    oss << "</table></body></html>";
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
