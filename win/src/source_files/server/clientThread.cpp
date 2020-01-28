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

            // data send end
            if ((parts.at(0) != "mcm" && parts.at(parts.size() - 1) == "end") || parts.size() == 1)
            {
                sendMessage(remote_ip, message, "DATA APPEND");
            }
            else
            {
                // message types
                if (parts.at(2) == "text")
                {
                    remote_ip = parts.at(3);

                    sendMessage(remote_ip, message, "TEXT");
                }
                else if (parts.at(2) == "next")
                {
                    sendMessage(remote_ip, message, "NEXT");
                }
                else
                {
                    if (parts.at(2) == "data")
                    {
                        remote_ip = parts.at(3);
                    }

                    sendMessage(remote_ip, message, "DATA");
                }
            }
        }
    }

    // Close the socket
    closesocket(mClientSocket);
}

void ClientThread::sendMessage(std::string ip, std::string& message, std::string info) const
{
    std::map<std::string, int>::iterator It;

    if (ip == "broadcast")
    {
        std::cout << info << " Broadcast to all connected clients" << std::endl;

        for (It = mpServer->mClients->begin(); It != mpServer->mClients->end(); It++)
        {
            if (It->first != mIp)
            {
                send(It->second, message.c_str(), message.size(), 0);
            }
        }
    }
    else
    {
        It = mpServer->mClients->find(ip);

        if (It != mpServer->mClients->end())
        {
            std::cout << info << " Redirect to " << ip << std::endl;

            send(It->second, message.c_str(), message.size(), 0);
        }
        else
        {
            std::cout << info << " No client with ip " << ip << " connected" << std::endl;
        }
    }
}