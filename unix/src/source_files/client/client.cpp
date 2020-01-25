#include "../../header_files/client/client.hpp"

#include "../../header_files/client/receiver.hpp"

Client::Client() :
    isOK{false},
    singleFile{false},
    offset{0}
{

}

void Client::start()
{
    int response;

    // Create socket
    int sock = socket(AF_INET, SOCK_STREAM, 0);

    if (sock == INVALID_SOCKET)
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
    
    if (response == SOCKET_ERROR)
    {
        std::cout << "Cant bind" << std::endl;
    }
    else
    {
        std::cout << "Connected" << std::endl;
    }

    Receiver receiver(sock);

    std::thread rec(receiver);

    rec.detach();

    sender();
    
    receiver.m_run = false;

    close();
}

void Client::sender()
{
    int response;

    while(true)
    {
        // reset variables
        isOK = true;
		
        if (singleFile == true)
        {
            remote_ip = "broadcast";
        }
        
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
            readPath();
        }
        else
        {
            message = "mcm:1.0:text:" + input;
            output = "mcm:1.0:text:" + input;

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

void WClient::sendNextFile(std::string ip)
{
    output = "mcm:1.0:send next:" + remote_ip + ":end:";
    message = "mcm:1.0:send next:" + remote_ip + ":end:";

    int response = send(sock, message.c_str(), message.size(), 0);

    if (response == SOCKET_ERROR)
    {
        std::cout << "Could not send message to server!" << std::endl;
    }
}

void Client::sendFile()
{
    if (singleFile == false)
    {
        if (files_It != m_Files.end())
        {
            if (std::filesystem::is_directory(files_It->c_str()) == true)
            {
                isDirectory = "true";
            }
            else
            {
                isDirectory = "false";
            }

            readFile(files_It->c_str());

            files_It++;
        }
        else
        {
            singleFile = true;
        }
    }
}

void Client::readPath()
{
    std::string filename{ "" };

    std::vector<std::string> parts{ Tools::split(input, ' ') };

    for (int i{ 1 }; i < parts.size(); i++)
    {
        if (parts.at(i).substr(0, 3) == "ip=")
        {
            remote_ip = parts.at(i).substr(3, parts.at(i).length() - 3);
        }
        else if (parts.at(i).substr(0, 5) == "file=")
        {
            filename = parts.at(i).substr(5, parts.at(i).length() - 5);

            std::vector<std::string> pathParts{ Tools::split(filename, '/') };

            offset = filename.size() - pathParts.at(pathParts.size() - 1).size();

            if (std::filesystem::is_directory(filename) == true)
            {
                singleFile = false;

                m_Files = Tools::readDirectory(filename);

                files_It = m_Files.begin();

                sendFile();
            }
            else
            {
                singleFile = true;

                isDirectory = "false";

                readFile(filename);
            }
        }
        else
        {
            std::cout << "Wrong parameter: " << parts.at(i) << std::endl;

            isOK = false;

            break;
        }                
    }

    output = "mcm:1.0:data send:" + remote_ip + ":" + filename + ":" + isDirectory + ":DATA:end:";

    std::cout << "Send message: " << output << std::endl;
}

void Client::readFile(std::string filename)
{
    // Read file

    std::string document{ "" };

    if (std::filesystem::is_directory(filename) == false)
    {
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

            sendFile(document, filename);
        }
        else
        {
            isOK = false;

            std::cout << "File not found" << std::endl;
        }
    }
    else
    {
        document = "null";

        sendFile(document, filename);
    }
}

void Client::sendFile(std::string& file, std::string filename)
{
    // Send to server
    if (isOK == true)
    {
        filename = filename.substr(offset, filename.size() - offset);

        message = "mcm:1.0:data send:" + remote_ip + ":" + filename + ":" + isDirectory + ":" + file + ":end:";

        int response = send(sock, message.c_str(), message.size(), 0);

        if (response == SOCKET_ERROR)
        {
            std::cout << "Could not send file to server!" << std::endl;
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