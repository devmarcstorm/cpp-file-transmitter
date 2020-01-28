#include "../../header_files/client/receiver.hpp"

#include "../../header_files/client/client.hpp"

#include "../../header_files/gui/textArea.hpp"

#include "../../header_files/display.hpp"

Receiver::Receiver(Client* client, int sock) :
    mp_client{client},
    m_sock{sock}
{

}

void Receiver::operator()(std::future<void> futureObj) const
{
    std::cout << "Start receiving" << std::endl;
	
    int received;

    bool fileTransfer{ false };

    std::string remote_ip{ "" };

    std::string file{ "" };
    std::string filename{ "" };
    std::string isDirectory{ "false" };

    char buffer[4096];

    while (futureObj.wait_for(std::chrono::milliseconds(1)) == std::future_status::timeout)
    {
        memset(buffer, 0, 4096); // Initialize/Cleanup buffer

        // Wait for response
        received = recv(m_sock, buffer, 4096, 0);

        if (received == -1)
        {
            std::cerr << "Error response from server" << std::endl;

            break;
        }
        else
        {
            std::string message{ std::string(buffer, 0, received) };
			
            // check message
            if (message != "")
            {
                std::vector<std::string> parts{ Tools::split(message, ':') };

                if (parts.size() > 1)
                {
                    if (parts.at(0) == "mcm" && parts.at(parts.size() - 1) == "end") // receiving full message
                    {
                        // message types
						if (parts.at(2) == "clients") // connected clients
                        {
                            std::string clients{ parts.at(3) };

                            std::vector<std::string> ips{ Tools::split(clients, ',') };

                            if (ips.empty())
                            {
                                std::dynamic_pointer_cast<TextArea>(Display::mGUIManager.getObject("console"))->addLine("There are no other clients connected", sf::Color::White);
                            }
                            else
                            {
                                for (int i{ 0 }; i < ips.size(); i++)
                                {
                                    std::dynamic_pointer_cast<TextArea>(Display::mGUIManager.getObject("console"))->addLine(ips.at(i), sf::Color::White);
                                }
                            }
                        }
                        else if (parts.at(2) == "text") // just text
                        {
                            std::string text{ parts.at(4) };

                            std::dynamic_pointer_cast<TextArea>(Display::mGUIManager.getObject("console"))->addLine(parts.at(4), sf::Color::White);
                        }
                        else if (parts.at(2) == "data") // file to send
                        {
                            setFileInfos(fileTransfer, remote_ip, filename, isDirectory, file, parts);

                            writeFile(file, filename, isDirectory);

                            fileTransfer = false;

                            mp_client->sendNextFile(remote_ip);
                        }
                        else if (parts.at(2) == "next")
                        {
                            mp_client->sendFile();
                        }
                    }
                    else if (parts.at(0) != "mcm" && parts.at(parts.size() - 1) == "end") // receiving data end
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
                            std::string text{ parts.at(4) };

                            std::dynamic_pointer_cast<TextArea>(Display::mGUIManager.getObject("console"))->addLine(parts.at(4), sf::Color::White);
                        }
                        else if (parts.at(2) == "data") // file to send
                        {
                            setFileInfos(fileTransfer, remote_ip, filename, isDirectory, file, parts);
                        }
                        else if (parts.at(2) == "next")
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