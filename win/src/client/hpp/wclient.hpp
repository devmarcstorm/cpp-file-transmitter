#ifndef WCLIENT_HPP
#define WCLIENT_HPP

#pragma comment(lib, "ws2_32.lib")  // Linker
#include <cstdio>
#include <iostream>
#include <WinSock2.h>
#include <Windows.h>
#include <fstream>
#include <string>
#include <vector>
#include <thread>

#include "../../Tools.hpp"
#include "../../Base64.hpp"

class WClient
{
public:

    WClient();

    void start();

    void sender();

    void close();

    ~WClient();

private:

    void readFile();
    void sendFile(std::string file, std::string filename);

    SOCKET sock;

    bool isOK;

    std::string remote_ip;
    std::string message;
    std::string output;
    std::string input;
};

#endif