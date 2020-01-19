#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>

#include "../../../Tools.hpp"
#include "../../../Base64.hpp"

void receive(SOCKET s)
{
    char buffer[4096];

    // Wait for response
    memset(buffer, 0, 4096);
    int received = recv(s, buffer, 4096, 0);

    if (received == SOCKET_ERROR)
    {
        std::cerr << "Error response from server" << std::endl;
    }
    else
    {
        std::string message = std::string(buffer, 0, received);

        // check message
        if (message != "")
        {
            std::vector<std::string> parts = Tools::Splitting(message, '/');

            // message types
            if (parts.at(2) == "text") // just text
            {
                std::string text = parts.at(3);

                std::cout << text << std::endl;
            }
            else if (parts.at(2) == "data send") // file to send
            {
                std::string file = parts.at(4);

                std::string result;

                macaron::Base64::Decode(file, result);

                FILE* f;

                f = fopen("test.txt", "wb");

                fwrite(result.c_str(), sizeof(char), result.size(), f);

                fclose(f);

                std::cout << "File received" << std::endl;
            }
        }
    }
}

int main()
{
    int response;

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
    response = connect(s, (sockaddr*)&hint, sizeof(hint));
    
    if (response == -1)
    {
        return 1;
    }

    char buffer[4096];
    std::string input;
    std::string message;
    std::string output;

    std::string remote_ip;
    std::string filename;
    std::string document;

    while(true)
    {
        // reset variables
        isOK = true;
        remote_ip = "broadcast";

        // user input
        std::cout << "> ";
        
        std::getline(std::cin, input);

        // Exit
        if (input == "!q" || input == "!Q" || input == "!Quit" || input == "!quit")
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

                    // Read file

                    std::string document;

                    unsigned int buffer[1024];

                    File* file;

                    f = fopen(filename, "rb");

                    if (f != nullptr)
                    {
                        while (!feof(f))
                        {
                            int bytes = fread(buffer, 1, 1024, f);

                            for (int i = 0; i < bytes; i++)
                            {
                                document += buffer[i];
                            }
                        }

                        fclose(f);

                        document = maracon::Base64::Encode(document);                    
                    }
                    else
                    {
                        isOK = false;
                        
                        std::cout << "File not found" << std::endl;
                    }

                    //std::vector<std::string> path = Tools::Split(filename, '/');

                    //filename = path.at(path.size() - 1);
                }
                else
                {
                    std::cout << "Wrong parameter: " << parts.at(i) << std::endl;

                    isOK = false;

                    break;
                }                
            }

            message = "mcm/1.0/data send/" + remote_ip + "/" + document + "/";
            output = "mcm/1.0/data send/" + remote_ip + "/" + filename + "/";
        }
        else
        {
            message = "mcm/1.0/text/" + input;
            output = "mcm/1.0/text/" + input;
        }        

        if (isOK == true)
        {
            // Send to server
            std::cout << "Message: " << message << std::endl;
            response = send(s, message.c_str(), message.size() + 1, 0);
            
            if (response == -1)
            {
                std::cout << "Could not send message to server!" << std::endl;
                continue;
            }
        }
        else
        {
            std::cout << "Could not send message: " << message << std::endl;
        }
    }

    // Close socket
    std::cout << "Shut down" << std::endl;

    close(s);

    return 0;
}