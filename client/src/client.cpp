#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>

#include "../../Tools.hpp"
#include "../../Base64.hpp"

int main()
{
    // Create socket
    int s = socket(AF_INET, SOCK_STREAM, 0);

    if (s == -1)
    {
        return 1;
    }

    // port and ip of the server
    int port = 20000;
    std::string ipAddress = "127.0.0.1";

    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(port);
    inet_pton(AF_INET, ipAddress.c_str(), &hint.sin_addr);

    // Connect to the server on the socket
    int connection = connect(s, (sockaddr*)&hint, sizeof(hint));
    
    if (connection == -1)
    {
        return 1;
    }

    int sendMessage;
    char buffer[4096];
    std::string input;
    std::string message;

    std::string remote_ip;
    std::string filename;

    while(true)
    {
        // user input
        std::cout << "> ";
        
        std::getline(std::cin, input);

        // Exit
        if (input == "!quit")
        {
            break;
        }

        // Send file
        if (input.substr(0, 2) == "-f")
        {
            std::vector<std::string> parts = Tools::Split(input, ' ');

            for (int i = 1; i < parts.size(); i++)
            {
                if (parts.at(i).substr(0, 3) == "ip=")
                {
                    remote_ip = parts.at(i).substr(3, parts.at(i).length() - 3);
                }
                else if (parts.at(i).substr(0, 5) == "file=")
                {
                    filename = parts.at(i).substr(5, parts.at(i).length() - 5);

                    std::vector<std::string> path = Tools::Split(filename, '/');

                    filename = path.at(path.size() - 1);
                }
                else
                {
                    std::cout << "Wrong parameter: " << parts.at(i) << std::endl;

                    message = "mcm/1.0/bad request";

                    break;
                }                
            }

            message = "mcm/1.0/data send/" + remote_ip + "/" + filename + "/";
        }
        else
        {
            message = "mcm/1.0/text/" + input;
        }
        

        // Send to server
        std::cout << "Message: " << message << std::endl;
        sendMessage = send(s, message.c_str(), message.size() + 1, 0);
        
        if (sendMessage == -1)
        {
            std::cout << "Could not send message to server!" << std::endl;
            continue;
        }

        // Wait for response
        memset(buffer, 0, 4096);
        int received = recv(s, buffer, 4096, 0);

        if (received == -1)
        {
            std::cerr << "Error response from server" << std::endl;
        }
        else
        {
            // Response
            std::cout << "Server: " << std::string(buffer, received) << std::endl;
        }
    }

    // Close socket
    std::cout << "Shut down" << std::endl;
    close(s);

    return 0;
}