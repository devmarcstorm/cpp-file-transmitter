#include "../../header_files/server/serverThread.hpp"

ServerThread::ServerThread(std::map<std::string, int>* clients) :
    mClients{ clients }
{

}

int ServerThread::Start()
{
    long response;

    // Versiondata
    WSADATA wsaData;

    response = WSAStartup(MAKEWORD(2, 0), &wsaData);

    if (response != 0)
    {
        std::cerr << "WSAStartup failed: " << WSAGetLastError() << std::endl;
    }

    // Create listener (server)
    mListener = socket(AF_INET, SOCK_STREAM, 0);

    if (mListener == INVALID_SOCKET)
    {
        std::cerr << "Cant create listener: " << WSAGetLastError() << std::endl;
    }

    // Connection informations
    sockaddr_in info;
    info.sin_addr.s_addr = inet_addr("0.0.0.0");
    info.sin_family = AF_INET;
    info.sin_port = htons(20000);

    response = bind(mListener, (struct sockaddr*)&info, sizeof(info));

    if (response == SOCKET_ERROR)
    {
        std::cerr << "Cant bind: " << WSAGetLastError() << std::endl;
    }

    return response;
}

ServerThread::~ServerThread()
{

}

void ServerThread::operator()(std::future<void> futureObj) const
{
    long response;

    // Listen
    response = listen(mListener, SOMAXCONN); 

    if (response == SOCKET_ERROR)
    {
        std::cerr << "Cant listen: " << WSAGetLastError() << std::endl;
    }
    else
    {
        std::cout << "Start listening..." << std::endl;
    }

    sockaddr_in clientinfo;
    int size{ sizeof(sockaddr_in) };

    fd_set read_fds;
    FD_ZERO(&read_fds);
    FD_SET(mListener, &read_fds);

    timeval timeout;
    timeout.tv_sec = 2;
    timeout.tv_usec = 0;

    int select_status;

    while (futureObj.wait_for(std::chrono::milliseconds(1)) == std::future_status::timeout)
    {
        // select() is non-blocking
        select_status = select(mListener, &read_fds, NULL, NULL, &timeout);

        if (select_status > 0)
        {
            SOCKET client = accept(mListener, (struct sockaddr*)& clientinfo, &size);

            char* hostname = inet_ntoa(clientinfo.sin_addr);

            if (client != SOCKET_ERROR)
            {
                std::cout << hostname << " connected" << std::endl;
            }
            else
            {
                std::cerr << "ERROR: " << WSAGetLastError() << std::endl;
            }

            ClientThread clientthread(hostname, (int)client, this);
            std::thread newClient(clientthread);

            newClient.detach();

            mClients->insert(std::pair<std::string, int>(hostname, client));

            std::cout << "Count of connected clients: " << mClients->size() << std::endl;
        }
    }

    Close();
}

void ServerThread::Close() const
{
    closesocket(mListener);

    WSACleanup();

    mClients->clear();

    std::cout << "Shut down server" << std::endl;
}