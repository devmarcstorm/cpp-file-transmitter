#ifndef SERVERTHREAD_HPP
#define SERVERTHREAD_HPP

#include <thread>
#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string>
#include <vector>
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

    int listener;
};

#endif