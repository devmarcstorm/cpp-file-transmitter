#include "../../header_files/client/client.hpp"

#include "../../header_files/client/receiver.hpp"

#include "../../header_files/gui/textArea.hpp"

#include "../../header_files/display.hpp"

Client::Client() :
    mIsOK{ false },
    mSingleFile{ false },
    mOffset{ 0 }
{

}

int Client::start(std::string ip)
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
    mSock = socket(AF_INET, SOCK_STREAM, 0);

    if (mSock == INVALID_SOCKET)
    {
        std::cerr << "Cant create socket: " << WSAGetLastError() << std::endl;
    }

    // Connection informations

    sockaddr_in info;
    info.sin_addr.s_addr = inet_addr(ip.c_str());   // Server ip
    info.sin_family = AF_INET;
    info.sin_port = htons(20000);                   // Server port

    response = connect(mSock, (struct sockaddr*)&info, sizeof(info));

    if (response == SOCKET_ERROR)
    {
        std::cout << "(ERROR) Cant connect: " << WSAGetLastError() << std::endl;

        std::dynamic_pointer_cast<TextArea>(Display::mGUIManager.getObject("console"))->addLine("Cant connect to: " + ip, sf::Color::Red);
    }
    else
    {
        std::dynamic_pointer_cast<TextArea>(Display::mGUIManager.getObject("console"))->addLine("Connected", sf::Color::Green);

        std::promise<void>().swap(mExitSignal);

        std::future<void> futureObj = mExitSignal.get_future();

        Receiver receiver(this, mSock);

        std::thread rec(receiver, std::move(futureObj));

        rec.detach();
    }

    mRemoteIp = "broadcast";

    return response;
}

void Client::sender(std::string input)
{
    if (input != "")
    {
        mInput = input;

        int response;

        // reset variables
        mIsOK = true;

        if (mInput.substr(0, 2) == "-i")
        {
            help();
        }
        else if (mInput.substr(0, 2) == "-f")
        {
            readPath();
        }
        else if (mInput.substr(0, 2) == "-s")
        {
            saveSettings();
        }
        else if (mInput.substr(0, 2) == "-r")
        {
            mRemoteIp = "broadcast";

            std::dynamic_pointer_cast<TextArea>(Display::mGUIManager.getObject("console"))->addLine("Reset remote ip to: broadcast (to all)", sf::Color::Magenta);
        }
        else
        {
            mMessage = "mcm:1.0:text:" + mRemoteIp + ":" + mInput + ":end:";
            mOutput = "mcm:1.0:text:" + mRemoteIp + ":" + mInput + ":end:";

            response = send(mSock, mMessage.c_str(), mMessage.size(), 0);

            if (response == SOCKET_ERROR)
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
}

void Client::saveSettings()
{
    std::vector<std::string> parts{ Tools::split(mInput, ' ') };

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
                mRemoteIp = parts.at(i).substr(3, parts.at(i).length() - 3);
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
    mOutput = "mcm:1.0:next:" + mRemoteIp + ":end:";
    mMessage = "mcm:1.0:next:" + mRemoteIp + ":end:";

    int response = send(mSock, mMessage.c_str(), mMessage.size(), 0);

    if (response == SOCKET_ERROR)
    {
        std::dynamic_pointer_cast<TextArea>(Display::mGUIManager.getObject("console"))->addLine("Could not send 'next' message!", sf::Color::Red);
    }
}

void Client::sendFile()
{
    if (mSingleFile == false)
    {
        if (mFilesIt != mFiles.end())
        {
            if (std::filesystem::is_directory(mFilesIt->c_str()) == true)
            {
                mIsDirectory = "true";
            }
            else
            {
                mIsDirectory = "false";
            }

            readFile(mFilesIt->c_str());

            mFilesIt++;
        }
        else
        {
            mSingleFile = true;
        }
    }
}

void Client::readPath()
{
    std::string filename{ "" };

    std::vector<std::string> parts{ Tools::split(mInput, ' ') };

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
                mRemoteIp = parts.at(i).substr(3, parts.at(i).length() - 3);
            }
            else if (parts.at(i).substr(0, 5) == "file=")
            {
                filename = parts.at(i).substr(5, parts.at(i).length() - 5);

                std::vector<std::string> pathParts{ Tools::split(filename, '/') };

                mOffset = filename.size() - pathParts.at(pathParts.size() - 1).size();

                if (std::filesystem::is_directory(filename) == true)
                {
                    mSingleFile = false;

                    mFiles = Tools::readDirectory(filename);

                    mFilesIt = mFiles.begin();

                    sendFile();
                }
                else
                {
                    mSingleFile = true;

                    mIsDirectory = "false";

                    readFile(filename);
                }
            }
            else
            {
                std::dynamic_pointer_cast<TextArea>(Display::mGUIManager.getObject("console"))->addLine("Wrong parameter: " + parts.at(i), sf::Color::Red);
                std::dynamic_pointer_cast<TextArea>(Display::mGUIManager.getObject("console"))->addLine("Syntax: -f ip=<ip> file=<filepath> or -f file=<filepath>", sf::Color::Magenta);

                mIsOK = false;

                break;
            }
        }

        if (mIsOK == true)
        {
            mOutput = "mcm:1.0:data:" + mRemoteIp + ":" + filename + ":" + mIsDirectory + ":DATA:end:";

            std::dynamic_pointer_cast<TextArea>(Display::mGUIManager.getObject("console"))->addLine("Send message: " + mOutput, sf::Color::Green);
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
            mIsOK = false;

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
    if (mIsOK == true)
    {
        filename = filename.substr(mOffset, filename.size() - mOffset);

        mMessage = "mcm:1.0:data:" + mRemoteIp + ":" + filename + ":" + mIsDirectory + ":" + file + ":end:";

        int response = send(mSock, mMessage.c_str(), mMessage.size(), 0);

        if (response == SOCKET_ERROR)
        {
            std::dynamic_pointer_cast<TextArea>(Display::mGUIManager.getObject("console"))->addLine("Could not send 'file'!", sf::Color::Red);
        }
    }
    else
    {
        std::dynamic_pointer_cast<TextArea>(Display::mGUIManager.getObject("console"))->addLine("Could not send message: " + mMessage, sf::Color::Red);
    }
}

void Client::close()
{
    std::cout << "Shut down client..." << std::endl;

    mExitSignal.set_value();

    closesocket(mSock);

    WSACleanup();
}

Client::~Client()
{
    
}