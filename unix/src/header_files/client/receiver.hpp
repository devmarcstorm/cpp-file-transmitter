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

    bool m_run;

private:

    void setFileInfos(bool& fileTransfer, std::string& remote_ip, std::string& filename, std::string& isDirectory, std::string& file, std::vector<std::string>& parts) const;

    void writeFile(std::string& file, std::string filename, std::string isDirectory) const;

    WClient* mp_client;

    SOCKET m_sock;
};

#endif