#ifndef WSERVERTHREAD_HPP
#define WSERVERTHREAD_HPP

#pragma comment(lib, "ws2_32.lib")  // Linker
#include <cstdio>
#include <iostream>
#include <WinSock2.h>
#include <Windows.h>
#include <string>
#include <vector>
#include <thread>
#include <map>

#include "wClientThread.hpp"

class WServerThread
{
public:
    WServerThread(std::map<std::string, int>* clients);

    int Start();

    ~WServerThread();

    void operator()() const;
    
    void Close();

    bool broken;

    std::map<std::string, int> *m_clients;

private:
    int listener;

    bool run;
};

#endif