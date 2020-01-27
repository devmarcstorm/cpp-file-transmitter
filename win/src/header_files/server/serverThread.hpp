#ifndef SERVERTHREAD_HPP
#define SERVERTHREAD_HPP

#pragma comment(lib, "ws2_32.lib")  // Linker
#include <cstdio>
#include <iostream>
#include <WinSock2.h>
#include <Windows.h>
#include <string>
#include <vector>
#include <thread>
#include <map>

#include <chrono>
#include <future>

#include "clientThread.hpp"

class ServerThread
{
public:

    ServerThread(std::map<std::string, int>* clients);

    int Start();

    ~ServerThread();

    void operator()(std::future<void> futureObj) const;
    
    void Close() const;

    std::map<std::string, int> *mClients;

private:

    int mListener;
};

#endif