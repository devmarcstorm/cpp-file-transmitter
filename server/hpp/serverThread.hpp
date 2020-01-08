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

#include "clientThread.hpp"

class ServerThread
{
public:
    ServerThread()
    {
        broken = false;
        run = false;

        if (Start() == -1)
        {
            broken = true;
        }
        else
        {
            run = true;
        }
    }

    int Start()
    {
        // create socket
        s = socket(AF_INET, SOCK_STREAM, 0);

        if (s == -1)
        {
            std::cerr << "ERROR: Can't create socket!" << std::endl;
            return -1;
        }

        // bind ip address port
        sockaddr_in sockAddr;
        sockAddr.sin_family = AF_INET;
        sockAddr.sin_port = htons(20000);

        inet_pton(AF_INET, "0.0.0.0", &sockAddr.sin_addr);
        bind(s, (sockaddr *)&sockAddr, sizeof(sockAddr));

        return 0;
    }

    ~ServerThread()
    {
    }

    void operator()() const
    {
        std::cout << "Listening..." << std::endl;

        // listening
        listen(s, SOMAXCONN);

        // wait for a connection
        sockaddr_in client;
        socklen_t clientSize = sizeof(client);

        while (run)
        {
            // get client
            int clientSocket = accept(s, (sockaddr *)&client, &clientSize);

            char hostname[NI_MAXHOST];
            char port[NI_MAXSERV];

            memset(hostname, 0, NI_MAXHOST);
            memset(port, 0, NI_MAXSERV);

            if (getnameinfo((sockaddr *)&client, sizeof(client), hostname, NI_MAXHOST, port, NI_MAXSERV, 0) == 0)
            {
                std::cout << "Client " << hostname << " connected! Port " << port << std::endl;
            }
            else
            {
                inet_ntop(AF_INET, &client.sin_addr, hostname, NI_MAXHOST);
                std::cout << "Client " << hostname << " connected! Port " << port << std::endl;
            }

            ClientThread clientthread(clientSocket);
            std::thread newClient(clientthread);

            newClient.detach();
        }

        std::cout << "Close socket" << std::endl;

        // Close socket
        close(s);
    }

    void Close()
    {
        run = false;
    }

    bool broken;

private:
    int s;

    bool run;
};

#endif