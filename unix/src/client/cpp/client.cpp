#include "../hpp/client.hpp"

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

Client::Client()
{

}

void Client::start()
{
    int response;

    // Create socket
    int s = socket(AF_INET, SOCK_STREAM, 0);

    if (s == -1)
    {
        std::cout << "Cant create socket" << std::endl;
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
        std::cout << "Cant bind" << std::endl;
    }
    else
    {
        std::cout << "Connected" << std::endl;
    }
    std::thread Receiver(receive, sock);

    sender();

    Receiver.join();

    close();
}

void Client::sender()
{
    int response;

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
            readFile();
        }
        else
        {
            message = "mcm/1.0/text/" + input;
            output = "mcm/1.0/text/" + input;

            std::cout << "Send message: " << output << std::endl;

            response = send(sock, message.c_str(), message.size(), 0);

            if (response == SOCKET_ERROR)
            {
                std::cout << "Could not send message to server!" << std::endl;
                continue;
            }
        }         
    }
}

void Client::readFile()
{
    std::string filename;
    std::string filetype;
    std::string document;

    std::vector<std::string> parts = Tools::Splitting(input, ' ');

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

            unsigned char buffer[1024];

            FILE* f;

            f = fopen(filename.c_str(), "rb");

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

                // Encoding
                document = macaron::Base64::Encode(document);
                
                // Get file extension
                std::vector<std::string> path = Tools::Splitting(filename, '/');

                filename = path.at(path.size() - 1);
            }
            else
            {
                isOK = false;
                        
                std::cout << "File not found" << std::endl;
            }
        }
        else
        {
            std::cout << "Wrong parameter: " << parts.at(i) << std::endl;

            isOK = false;

            break;
        }                
    }

    output = "mcm/1.0/data send/" + remote_ip + "/" + filename + "/DATA/end/";

    sendFile(document, filename);
}

void Client::sendFile(std::string& file, std::string filename)
{
    // Send to server
    if (isOK == true)
    {
        std::cout << "Send message: " << output << std::endl;

        message = "mcm/1.0/data send/" + remote_ip + "/" + filename + "/";

        int response = send(sock, message.c_str(), message.size(), 0);

        if (response == SOCKET_ERROR)
        {
            std::cout << "Could not send prefix of file transfer to server!" << std::endl;
        }

        response = send(sock, file.c_str(), file.size(), 0);

        if (response == SOCKET_ERROR)
        {
            std::cout << "Could not send file to server!" << std::endl;
        }

        response = send(sock, "/end/", 5, 0);

        if (response == SOCKET_ERROR)
        {
            std::cout << "Could not send suffix of file transfer to server!" << std::endl;
        }
    }
    else
    {
        std::cout << "Could not send message: " << message << std::endl;
    }
}

void Client::close()
{
    std::cout << "Shut down client..." << std::endl;   

    close(sock);
}

Client::~Client()
{
    
}