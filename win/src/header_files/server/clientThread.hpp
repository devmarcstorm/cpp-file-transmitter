#ifndef CLIENTTHREAD_HPP
#define CLIENTTHREAD_HPP

#include "../Tools.hpp"
#include "../Base64.hpp"

class ServerThread;

class ClientThread
{
public:

    ClientThread(char* ip, int clientSocket, const ServerThread* server);

    ~ClientThread();

    void operator()() const;

private:

    char* mIp;

    int mClientSocket;

    const ServerThread* mpServer;
};

#endif