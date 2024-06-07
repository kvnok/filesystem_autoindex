#pragma once

#define PORT 8080

string generateDirectoryListing(const string& path);
void handleClient(int clientSocket);
