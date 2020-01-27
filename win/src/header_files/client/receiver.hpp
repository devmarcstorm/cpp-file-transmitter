#ifndef RECEIVER_HPP
#define RECEIVER_HPP

#pragma comment(lib, "ws2_32.lib")  // Linker
#include <cstdio>
#include <iostream>
#include <WinSock2.h>
#include <Windows.h>
#include <fstream>
#include <string>
#include <vector>
#include <thread>

#include <chrono>
#include <future>

#include "../Tools.hpp"
#include "../Base64.hpp"

class Client;

class Receiver
{
public:

    Receiver(Client* client, SOCKET sock);

    void operator()(std::future<void> futureObj) const;

    ~Receiver();

private:

    void setFileInfos(bool& fileTransfer, std::string& remote_ip, std::string& filename, std::string& isDirectory, std::string& file, std::vector<std::string>& parts) const;

    void writeFile(std::string& file, std::string filename, std::string isDirectory) const;

    Client* mpClient;

    SOCKET mSock;
};

#endif