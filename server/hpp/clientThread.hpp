#ifndef CLIENTTHREAD_HPP
#define CLIENTTHREAD_HPP

#include "../../Tools.hpp"
#include "../../Base64.hpp"

class ClientThread
{
public:

    ClientThread(int clientSocket)
    {
        std::cout << "New client created" << std::endl;

        m_clientSocket = clientSocket;
    }

    ~ClientThread()
    {

    }

    void operator()() const
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
                std::cout << "Client disconnected " << std::endl;
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
                }
            }

            // Echo message back to client
            send(m_clientSocket, buffer, received + 1, 0);
        }

        // Close the socket
        close(m_clientSocket);
    }

private:

    int m_clientSocket;
};

#endif