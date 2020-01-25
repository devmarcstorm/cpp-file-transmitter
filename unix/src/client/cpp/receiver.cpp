#include "../hpp/receiver.hpp"

Receiver::Receiver(Client* client, SOCKET sock) :
    mp_client{client},
    m_sock{sock},
    m_run{true}
{

}

void Receiver::operator()() const
{
    int received;

    bool fileTransfer{ false };

    std::string remote_ip{ "" };

    std::string file{ "" };
    std::string filename{ "" };
    std::string isDirectory{ "false" };

    char buffer[4096];

    while (m_run)
    {
        memset(buffer, 0, 4096); // Initialize/Cleanup buffer

        // Wait for response
        received = recv(m_sock, buffer, 4096, 0);

        if (received == SOCKET_ERROR)
        {
            std::cerr << "Error response from server" << std::endl;

            break;
        }
        else
        {
            std::string message{ std::string(buffer, 0, received) };

            std::cout << message << std::endl;
            std::cout << std::endl;

            // check message
            if (message != "")
            {
                std::vector<std::string> parts{ Tools::split(message, ':') };

                if (parts.size() > 1)
                {
                    if (parts.at(0) == "mcm" && parts.at(parts.size() - 1) == "end") // receiving full message
                    {
                        // message types
                        if (parts.at(2) == "text") // just text
                        {
                            std::string text{ parts.at(3) };
                        }
                        else if (parts.at(2) == "data send") // file to send
                        {
                            setFileInfos(fileTransfer, remote_ip, filename, isDirectory, file, parts);

                            writeFile(file, filename, isDirectory);

                            fileTransfer = false;

                            mp_client->sendNextFile(remote_ip);
                        }
                        else if (parts.at(2) == "send next")
                        {
                            mp_client->sendFile();
                        }
                    }

                    if (parts.at(0) != "mcm" && parts.at(parts.size() - 1) == "end") // receiving data end
                    {
                        if (fileTransfer == false)
                        {
                            setFileInfos(fileTransfer, remote_ip, filename, isDirectory, file, parts);
                        }
                        else if (parts.size() > 1)
                        {
                            file += parts.at(0);
                        }

                        writeFile(file, filename, isDirectory);

                        fileTransfer = false;

                        mp_client->sendNextFile(remote_ip);
                    }
                    else // receiving begin message
                    {
                        // message types
                        if (parts.at(2) == "text") // just text
                        {
                            std::string text{ parts.at(3) };
                        }
                        else if (parts.at(2) == "data send") // file to send
                        {
                            setFileInfos(fileTransfer, remote_ip, filename, isDirectory, file, parts);
                        }
                        else if (parts.at(2) == "send next")
                        {
                            mp_client->sendFile();
                        }
                    }
                }
                else // receiving data part
                {
                    if (fileTransfer == true)
                    {
                        file += parts.at(0);
                    }
                }
            }
        }
    }

    std::cout << "Stop receiving" << std::endl;
}


void Receiver::setFileInfos(bool& fileTransfer, std::string& remote_ip, std::string& filename, std::string& isDirectory, std::string& file, std::vector<std::string>& parts) const
{
    fileTransfer = true;

    remote_ip = parts.at(3);

    filename = parts.at(4);

    isDirectory = parts.at(5);

    file = parts.at(6);
}

void Receiver::writeFile(std::string& file, std::string filename, std::string isDirectory) const
{
	if (mkdir("received", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) != -1)
	{
        // create path
        std::vector<std::string> pathParts{ Tools::split(filename, '/') };

        int count{ (int)pathParts.size() };

        if (isDirectory == "false")
        {
            count -= 1;
        }

        for (int i{ 0 }; i < count; i++)
        {
            std::string newpath{ "received/" + Tools::concatenate(pathParts, i + 1) };

            mkdir(newpath.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
        }

        if (isDirectory == "false")
        {
            std::string result;

            std::string error{ macaron::Base64::Decode(file, result) };

            if (error != "")
            {
                std::cout << error << std::endl;
            }

            // create file
            FILE* f;

            f = fopen(std::string("received/" + filename).c_str(), "wb");

            fwrite(result.c_str(), sizeof(char), result.size(), f);

            fclose(f);
        }
    }
    else
    {
        std::cout << "Cant create dirctory" << std::endl;
    }
}

Receiver::~Receiver()
{

}