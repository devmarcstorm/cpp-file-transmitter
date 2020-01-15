#include "../hpp/clientThread.hpp"

#include "../hpp/serverThread.hpp"

    ClientThread::ClientThread(char* ip, int clientSocket, const ServerThread* server)
    {
        m_ip = ip;

        m_clientSocket = clientSocket;

        mp_server = server;

        std::cout << "New client created" << std::endl;
    }

    ClientThread::~ClientThread()
    {
        
    }

    void ClientThread::operator()() const
    {
        std::cout << "Client: " << m_clientSocket << " start receiving" << std::endl;

        // While loop: accept and echo message back to client
        char buffer[4096];

        while (true)
        {
            memset(buffer, 0, 4096); //Sets the first num bytes of the block of memory pointed by ptr to the specified value (interpreted as an unsigned char).

            // Wait for client to send data
            int received = recv(m_clientSocket, buffer, 4096, 0);

            if (received == -1)
            {
                std::cerr << "Error in recv(). Quitting" << std::endl;
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

                std::cout << "Client disconnected" << std::endl;

                std::cout << "Count of connected clients: " << mp_server->m_clients->size() << std::endl;

                break;
            }

            std::string message = std::string(buffer, 0, received);

            std::cout << message << std::endl;

            // check message
            if (message != "")
            {
                std::vector<std::string> parts = Tools::Split(message, '/');

                // message types
                if (parts.at(2) == "text") // just text
                {
                    std::string text = parts.at(3);

                    std::cout << "Just text " << text << std::endl;
                }
                else if (parts.at(2) == "data send") // file to send
                {
                    std::string remote_ip = parts.at(3);
                    std::string filename = parts.at(4);

                    std::cout << "Data send: ip: " << remote_ip << " file: " << filename << std::endl;

                    // find the client of the remote_ip
                    std::map<std::string, int>::iterator It;

                    It = mp_server->m_clients->find(remote_ip);

                    if (It != mp_server->m_clients->end())
                    {
                        std::cout << "Redirect to " << remote_ip << std::endl;

                        send(It->second, buffer, received + 1, 0);
                    }
                    else
                    {
                        std::cout << "No client with ip " << remote_ip << " connected" << std::endl;
                    }
                }
            }

            // Echo message back to client
            send(m_clientSocket, buffer, received + 1, 0);
        }

        // Close the socket
        close(m_clientSocket);
    }