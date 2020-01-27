#include "../../header_files/server/clientThread.hpp"

#include "../../header_files/server/serverThread.hpp"

ClientThread::ClientThread(char* ip, int clientSocket, const ServerThread* server) :
    mIp{ ip },
    mClientSocket{ clientSocket },
    mpServer{ server }
{
    std::cout << "New client created" << std::endl;
}

ClientThread::~ClientThread()
{

}

void ClientThread::operator()() const
{
    std::cout << "Client: " << mClientSocket << " start receiving" << std::endl;

    char buffer[4096];

    std::string remote_ip{ "" };

    while (true)
    {
        memset(buffer, 0, 4096); // Initialize/Cleanup buffer

        // Wait for client to send data
        int received = recv(mClientSocket, buffer, 4096, 0);

        if (received == SOCKET_ERROR)
        {
            std::cerr << mIp << " disconneted" << std::endl;
            break;
        }

        if (received == 0)
        {
            // remove client from list
            std::map<std::string, int>::iterator It;

            It = mpServer->mClients->find(mIp);

            if (It != mpServer->mClients->end())
            {
                It = mpServer->mClients->erase(It);
            }
            else
            {
                std::cout << "Some mysterious error occurs" << std::endl;
            }

            std::cout << mIp << " disconnected" << std::endl;

            std::cout << "Count of connected clients: " << mpServer->mClients->size() << std::endl;

            break;
        }

        std::string message{ std::string(buffer, 0, received) };

        // check message
        if (message != "")
        {
            std::vector<std::string> parts{ Tools::split(message, ':') };

            if ((parts.at(0) != "mcm" && parts.at(parts.size() - 1) == "end") || parts.size() == 1)
            {
                std::map<std::string, int>::iterator It;

                if (remote_ip == "broadcast")
                {
                    std::cout << "Broadcast 'data append' to all connected clients" << std::endl;

                    // send data to all clients
                    for (It = mpServer->mClients->begin(); It != mpServer->mClients->end(); It++)
                    {
                        // but not to the source
                        if (It->first != mIp)
                        {
                            send(It->second, buffer, sizeof(buffer), 0);
                        }
                    }
                }
                else
                {
                    // find the client of the remote_ip
                    It = mpServer->mClients->find(remote_ip);

                    if (It != mpServer->mClients->end())
                    {
                        std::cout << "Redirect 'data append' to " << remote_ip << std::endl;

                        send(It->second, buffer, sizeof(buffer), 0);
                    }
                    else
                    {
                        std::cout << "(DATA APPEND) No client with ip " << remote_ip << " connected" << std::endl;
                    }
                }
            }
            else
            {
                // message types
                if (parts.at(2) == "text") // text for the server
                {
                    std::string text{ parts.at(3) };

                    // Echo message back to client
                    send(mClientSocket, buffer, sizeof(buffer), 0);
                }
                else if (parts.at(2) == "send next")
                {
                    // find the client of the remote_ip
                    std::map<std::string, int>::iterator It{ mpServer->mClients->find(remote_ip) };

                    if (It != mpServer->mClients->end())
                    {
                        std::cout << "Redirect 'next' to " << remote_ip << std::endl;

                        send(It->second, buffer, sizeof(buffer), 0);
                    }
                    else
                    {
                        std::cout << "(NEXT) No client with ip " << remote_ip << " connected" << std::endl;
                    }

                    // Echo message back to client
                    send(It->second, buffer, sizeof(buffer), 0);
                }
                else
                {
                    if (parts.at(2) == "data send")
                    {
                        remote_ip = parts.at(3);
                    }

                    std::map<std::string, int>::iterator It;

                    if (remote_ip == "broadcast")
                    {
                        std::cout << "Broadcast 'data' to all connected clients" << std::endl;

                        // send data to all clients
                        for (It = mpServer->mClients->begin(); It != mpServer->mClients->end(); It++)
                        {
                            // but not to the source
                            if (It->first != mIp)
                            {
                                send(It->second, buffer, sizeof(buffer), 0);
                            }
                        }
                    }
                    else
                    {
                        // find the client of the remote_ip
                        It = mpServer->mClients->find(remote_ip);

                        if (It != mpServer->mClients->end())
                        {
                            std::cout << "Redirect 'data' to " << remote_ip << std::endl;

                            send(It->second, buffer, sizeof(buffer), 0);
                        }
                        else
                        {
                            std::cout << "(DATA) No client with ip " << remote_ip << " connected" << std::endl;
                        }
                    }
                }
            }
        }
    }

    // Close the socket
    closesocket(mClientSocket);
}