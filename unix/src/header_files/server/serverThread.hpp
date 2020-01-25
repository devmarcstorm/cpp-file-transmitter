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

#include "clientThread.hpp"

class ServerThread
{
public:
    ServerThread(std::map<std::string, int>* clients);

    int Start();

    ~ServerThread();

    void operator()() const;
    
    void Close();

    bool broken;

    std::map<std::string, int> *m_clients;

private:
    int listener;

    bool run;
};

#endif