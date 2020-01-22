#ifndef RECEIVER_HPP
#define RECEIVER_HPP

#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>
#include <thread>

#include "../../Tools.hpp"
#include "../../Base64.hpp"

class Receiver
{
public:

    Receiver(SOCKET sock);

    void operator()() const;

    ~Receiver();

    bool m_run;

private:

    SOCKET m_sock;
};

#endif