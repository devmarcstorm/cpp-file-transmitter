#ifndef WCLIENTTHREAD_HPP
#define WCLIENTTHREAD_HPP

#include "../../../Tools.hpp"
#include "../../../Base64.hpp"

class WServerThread;

class WClientThread
{
public:

    WClientThread(char* ip, int clientSocket, const WServerThread* server);

    ~WClientThread();

    void operator()() const;

private:

    char* m_ip;

    int m_clientSocket;

    const WServerThread* mp_server;
};

#endif