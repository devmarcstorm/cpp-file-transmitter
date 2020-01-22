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

#include "../../Tools.hpp"
#include "../../Base64.hpp"

class WReceiver
{
public:

    WReceiver(SOCKET sock);

    void operator()() const;

    ~WReceiver();

    bool m_run;

private:

    SOCKET m_sock;
};

#endif