#include "../../header_files/server/wClientThread.hpp"

#include "../../header_files/server/wServerThread.hpp"

WClientThread::WClientThread(char* ip, int clientSocket, const WServerThread* server) :
    m_ip{ip},
    m_clientSocket{clientSocket},
    mp_server{server}
{
    std::cout << "New client created" << std::endl;
}

WClientThread::~WClientThread()
{

}

void WClientThread::operator()() const
{
    std::cout << "Client: " << m_clientSocket << " start receiving" << std::endl;

    char buffer[4096];

    std::string remote_ip{ "" };

    while (true)
    {
        memset(buffer, 0, 4096); // Initialize/Cleanup buffer

        // Wait for client to send data
        int received = recv(m_clientSocket, buffer, 4096, 0);

        if (received == SOCKET_ERROR)
        {
            std::cerr << m_ip << " disconneted" << std::endl;
            break;
        }

        if (received == 0)
        {
            // remove client from list
            std::map<std::string, int>::iterator It;

            It = mp_server->m_clients->find(m_ip);

            if (It != mp_server->m_clients->end())
            {
                It = mp_server->m_clients->erase(It);
            }
            else
            {
                std::cout << "Some mysterious error occurs" << std::endl;
            }

            std::cout << m_ip << " disconnected" << std::endl;

            std::cout << "Count of connected clients: " << mp_server->m_clients->size() << std::endl;

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
                    for (It = mp_server->m_clients->begin(); It != mp_server->m_clients->end(); It++)
                    {
                        // but not to the source
                        if (It->first != m_ip)
                        {
                            send(It->second, buffer, sizeof(buffer), 0);
                        }
                    }
                }
                else
                {
                    // find the client of the remote_ip
                    It = mp_server->m_clients->find(remote_ip);

                    if (It != mp_server->m_clients->end())
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

                    std::cout << text << std::endl;

                    // Echo message back to client
                    send(m_clientSocket, buffer, sizeof(buffer), 0);
                }
                else if (parts.at(2) == "send next")
                {
                    // find the client of the remote_ip
                    std::map<std::string, int>::iterator It{ mp_server->m_clients->find(remote_ip) };

                    if (It != mp_server->m_clients->end())
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
                        for (It = mp_server->m_clients->begin(); It != mp_server->m_clients->end(); It++)
                        {
                            // but not to the source
                            if (It->first != m_ip)
                            {
                                send(It->second, buffer, sizeof(buffer), 0);
                            }
                        }
                    }
                    else
                    {
                        // find the client of the remote_ip
                        It = mp_server->m_clients->find(remote_ip);

                        if (It != mp_server->m_clients->end())
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
    closesocket(m_clientSocket);
}