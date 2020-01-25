#ifndef WRECEIVER_HPP
#define WRECEIVER_HPP

#pragma comment(lib, "ws2_32.lib")  // Linker
#include <cstdio>
#include <iostream>
#include <WinSock2.h>
#include <Windows.h>
#include <fstream>
#include <string>
#include <vector>
#include <thread>

#include "../Tools.hpp"
#include "../Base64.hpp"

class WClient;

class WReceiver
{
public:

    WReceiver(WClient* client, SOCKET sock);

    void operator()() const;

    ~WReceiver();

    bool m_run;

private:

    void setFileInfos(bool& fileTransfer, std::string& remote_ip, std::string& filename, std::string& isDirectory, std::string& file, std::vector<std::string>& parts) const;

    void writeFile(std::string& file, std::string filename, std::string isDirectory) const;

    WClient* mp_client;

    SOCKET m_sock;
};

#endif