#include "../../header_files/client/client.hpp"

#include "../../header_files/client/receiver.hpp"

#include "../../header_files/gui/textArea.hpp"

#include "../../header_files/display.hpp"

Client::Client() :
    isOK{false},
    singleFile{false},
    offset{0}
{

}

int Client::start(std::string ip)
{
    int response;

    // Create socket
    sock = socket(AF_INET, SOCK_STREAM, 0);

    if (sock == 0)
    {
        std::cout << "Cant create socket" << std::endl;
    }

    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(20000);
    inet_pton(AF_INET, ip.c_str(), &hint.sin_addr);

    // Connect to the server on the socket
    response = connect(sock, (sockaddr*)&hint, sizeof(hint));
    
    if (response == -1)
    {
        std::cout << "(ERROR) Cant connect" << std::endl;

        std::dynamic_pointer_cast<TextArea>(Display::mGUIManager.getObject("console"))->addLine("Cant connect to: " + ip, sf::Color::Red);
    }
    else
    {
        std::dynamic_pointer_cast<TextArea>(Display::mGUIManager.getObject("console"))->addLine("Connected", sf::Color::Green);

        std::promise<void>().swap(mExitSignal);

        std::future<void> futureObj = mExitSignal.get_future();

        Receiver receiver(this, sock);

        std::thread rec(receiver, std::move(futureObj));

        rec.detach();
    }
	
	remote_ip = "broadcast";

	return response;
}

void Client::sender(std::string command)
{
    if (command != "")
    {
        input = command;

        int response;

        // reset variables
        isOK = true;

        if (input.substr(0, 2) == "-i")
        {
            help();
        }
		else if (input.substr(0, 2) == "-c")
        {
            message = "mcm:1.0:clients:end:";
            output = "mcm:1.0:clients:end:";

            response = send(sock, message.c_str(), message.size(), 0);

            if (response == -1)
            {
                std::dynamic_pointer_cast<TextArea>(Display::mGUIManager.getObject("console"))->addLine("Could not send 'clients' message!", sf::Color::Red);
            }
        }
        else if (input.substr(0, 2) == "-f")
        {
            readPath();
        }
        else if (input.substr(0, 2) == "-s")
        {
            saveSettings();
        }
        else if (input.substr(0, 2) == "-r")
        {
            remote_ip = "broadcast";

            std::dynamic_pointer_cast<TextArea>(Display::mGUIManager.getObject("console"))->addLine("Reset remote ip to: broadcast (to all)", sf::Color::Magenta);
        }
        else
        {
            message = "mcm:1.0:text:" + remote_ip + ":" + input + ":end:";
            output = "mcm:1.0:text:" + remote_ip + ":" + input + ":end:";

            response = send(sock, message.c_str(), message.size(), 0);

            if (response == -1)
            {
                std::dynamic_pointer_cast<TextArea>(Display::mGUIManager.getObject("console"))->addLine("Could not send 'text' message!", sf::Color::Red);
            }
        }
    }
}

void Client::help()
{
    std::dynamic_pointer_cast<TextArea>(Display::mGUIManager.getObject("console"))->addLine("-s ip=<ip> << set remote ip", sf::Color::Magenta);
    std::dynamic_pointer_cast<TextArea>(Display::mGUIManager.getObject("console"))->addLine("-r << reset remote ip", sf::Color::Magenta);
    std::dynamic_pointer_cast<TextArea>(Display::mGUIManager.getObject("console"))->addLine("-f ip=<ip> file=<filepath> or -f file=<filepath> << send file", sf::Color::Magenta);
    std::dynamic_pointer_cast<TextArea>(Display::mGUIManager.getObject("console"))->addLine("-c << shows every connected client", sf::Color::Magenta);
}

void Client::saveSettings()
{
    std::vector<std::string> parts{ Tools::split(input, ' ') };

    if (parts.size() == 1)
    {
        std::dynamic_pointer_cast<TextArea>(Display::mGUIManager.getObject("console"))->addLine("Syntax: -s ip=<ip>", sf::Color::Magenta);
    }
    else
    {
        for (int i{ 1 }; i < parts.size(); i++)
        {
            if (parts.at(i).substr(0, 3) == "ip=")
            {
                remote_ip = parts.at(i).substr(3, parts.at(i).length() - 3);
            }
            else
            {
                std::dynamic_pointer_cast<TextArea>(Display::mGUIManager.getObject("console"))->addLine("Wrong parameter: " + parts.at(i), sf::Color::Red);
                std::dynamic_pointer_cast<TextArea>(Display::mGUIManager.getObject("console"))->addLine("Syntax: -s ip=<ip>", sf::Color::Magenta);
            }
        }
    }
}

void Client::sendNextFile(std::string ip)
{
    output = "mcm:1.0:next:" + remote_ip + ":end:";
    message = "mcm:1.0:send next:" + remote_ip + ":end:";

    int response = send(sock, message.c_str(), message.size(), 0);

    if (response == -1)
    {
        std::dynamic_pointer_cast<TextArea>(Display::mGUIManager.getObject("console"))->addLine("Could not send 'next' message!", sf::Color::Red);
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

    if (parts.size() == 1)
    {
        std::dynamic_pointer_cast<TextArea>(Display::mGUIManager.getObject("console"))->addLine("Syntax: -f ip=<ip> file=<filepath> or -f file=<filepath>", sf::Color::Magenta);
    }
    else
    {
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
                std::dynamic_pointer_cast<TextArea>(Display::mGUIManager.getObject("console"))->addLine("Wrong parameter: " + parts.at(i), sf::Color::Red);
                std::dynamic_pointer_cast<TextArea>(Display::mGUIManager.getObject("console"))->addLine("Syntax: -f ip=<ip> file=<filepath> or -f file=<filepath>", sf::Color::Magenta);

				isOK = false;

				break;
			}                
		}

		if (isOK == true)
		{
			output = "mcm:1.0:data send:" + remote_ip + ":" + filename + ":" + isDirectory + ":DATA:end:";

			std::dynamic_pointer_cast<TextArea>(Display::mGUIManager.getObject("console"))->addLine("Send message: " + output, sf::Color::Green);
		}
	}
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

            std::dynamic_pointer_cast<TextArea>(Display::mGUIManager.getObject("console"))->addLine("File not found: " + filename, sf::Color::Red);
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

        message = "mcm:1.0:data:" + remote_ip + ":" + filename + ":" + isDirectory + ":" + file + ":end:";

        int response = send(sock, message.c_str(), message.size(), 0);

        if (response == -1)
        {
            std::dynamic_pointer_cast<TextArea>(Display::mGUIManager.getObject("console"))->addLine("Could not send 'file'!", sf::Color::Red);
        }
    }
    else
    {
        std::dynamic_pointer_cast<TextArea>(Display::mGUIManager.getObject("console"))->addLine("Could not send message: " + message, sf::Color::Red);
    }
}
void Client::Close()
{
    std::cout << "Shut down client..." << std::endl;   

    mExitSignal.set_value();

    close(sock);
}

Client::~Client()
{
    
}