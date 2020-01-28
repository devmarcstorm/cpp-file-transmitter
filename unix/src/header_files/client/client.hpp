#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>

#include <chrono>
#include <future>

#include "../Tools.hpp"
#include "../Base64.hpp"

class Client
{
public:

    Client();

    int start(std::string ip);

    void sender(std::string input);

    void close();
	
	void sendNextFile(std::string ip);

    void sendFile();

    ~Client();

private:

    void help();

    void saveSettings();

    void readPath();
    void readFile(std::string filename);
    void sendFile(std::string& file, std::string filename);

    SOCKET sock;

    bool isOK;
    bool singleFile;

    std::string isDirectory;
    std::string remote_ip;
    std::string message;
    std::string output;
    std::string input;

    int offset;

    std::vector<std::string> m_Files;
    std::vector<std::string>::iterator files_It;
};

#endif