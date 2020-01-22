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

#include "../../Tools.hpp"
#include "../../Base64.hpp"

class Client
{
public:

    Client();

    void start();

    void sender();

    void close();

    ~Client();

private:

    void readFile();
    void sendFile(std::string& file, std::string filename);

    SOCKET sock;

    bool isOK;

    std::string remote_ip;
    std::string message;
    std::string output;
    std::string input;
};

#endif