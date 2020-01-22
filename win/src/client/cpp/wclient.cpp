#include "../hpp/wclient.hpp"

#include "../hpp/wreceiver.hpp"

WClient::WClient()
{

}

void WClient::start()
{
   int response;

    // Versiondata
    WSADATA wsaData;

    response = WSAStartup(MAKEWORD(2, 0), &wsaData);

    if (response != 0)
    {
        std::cerr << "WSAStartup failed: " << WSAGetLastError() << std::endl;
    }

    // Create socket
    sock = socket(AF_INET, SOCK_STREAM, 0);

    if (sock == INVALID_SOCKET)
    {
        std::cerr << "Cant create socket: " << WSAGetLastError() << std::endl;
    }

    // Connection informations

    sockaddr_in info;
    info.sin_addr.s_addr = inet_addr("127.0.0.1");  // Server ip
    info.sin_family = AF_INET;
    info.sin_port = htons(20000);                   // Server port

    response = connect(sock, (struct sockaddr*)&info, sizeof(info));

    if (response == SOCKET_ERROR)
    {
        std::cerr << "Cant bind: " << WSAGetLastError() << std::endl;
    }
    else
    {
        std::cout << "Connected" << std::endl;
    }

    WReceiver receiver(sock);

    std::thread rec(receiver);

    rec.detach();

    sender();
    
    receiver.m_run = false;

    close();
}

void WClient::sender()
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

void WClient::readFile()
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

            if (filename.find_last_of(".") == std::string::npos)
            {
                // Directory..
            }
            else
            {
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
        }
        else
        {
            std::cout << "Wrong parameter: " << parts.at(i) << std::endl;

            isOK = false;

            break;
        }                
    }

    output = "mcm:1.0:data send:" + remote_ip + ":" + filename + ":DATA:end:";

    sendFile(document, filename);
}

void WClient::sendFile(std::string file, std::string filename)
{
    // Send to server
    if (isOK == true)
    {
        std::cout << "Send message: " << output << std::endl;

        message = "mcm:1.0:data send:" + remote_ip + ":" + filename + ":" + file + ":end:";

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

void WClient::close()
{
    std::cout << "Shut down client..." << std::endl;   

    closesocket(sock);

    WSACleanup();
}

WClient::~WClient()
{
    
}