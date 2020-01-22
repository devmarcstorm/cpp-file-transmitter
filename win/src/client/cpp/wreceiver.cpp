#include "../hpp/wreceiver.hpp"

WReceiver::WReceiver(SOCKET sock)
{
    m_sock = sock;

    m_run = true;
}

void WReceiver::operator()() const
{
    int received;

    bool fileTransfer = false;

    std::string file = "";
    std::string filename = "";

    char buffer[4096];

    while (m_run)
    {
        memset(buffer, 0, 4096);

        // Wait for response
        received = recv(m_sock, buffer, 4096, 0);

        if (received == SOCKET_ERROR)
        {
            std::cerr << "Error response from server" << std::endl;

            break;
        }
        else
        {
            std::string message = std::string(buffer, 0, received);

            // check message
            if (message != "")
            {
                std::vector<std::string> parts = Tools::Splitting(message, ':');

                if (parts.at(parts.size() - 1) == "end")
                {
                    file += parts.at(0);

                    std::string result;

                    std::string error = macaron::Base64::Decode(file, result);

                    std::cout << error << std::endl;

                    if (CreateDirectory("received", NULL) || ERROR_ALREADY_EXISTS == GetLastError())
                    {
                        FILE* f;

                        f = fopen(std::string("received/" + filename).c_str(), "wb");

                        fwrite(result.c_str(), sizeof(char), result.size(), f);

                        fclose(f);
                    }
                    else
                    {
                        std::cout << "Cant create dirctory" << std::endl;
                    }

                    fileTransfer = false;
                }
                else
                {
                    if (parts.size() > 1)
                    {
                        // message types
                        if (parts.at(2) == "text") // just text
                        {
                            std::string text = parts.at(3);
                        }
                        else if (parts.at(2) == "data send") // file to send
                        {
                             fileTransfer = true;

                             filename = parts.at(4);

                             file = parts.at(5);
                        }
                    }
                    else
                    {
                        if (fileTransfer == true)
                        {
                            file += parts.at(0);
                        }
                    }
                }
            }
        }
    }

    std::cout << "Stop receiving" << std::endl;
}

WReceiver::~WReceiver()
{

}