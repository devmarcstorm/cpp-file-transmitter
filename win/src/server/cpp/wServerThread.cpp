#include "../hpp/wServerThread.hpp"

WServerThread::WServerThread(std::map<std::string, int>* clients)
{
    m_clients = clients;

    broken = false;
    run = false;

    if (Start() == -1)
    {
        broken = true;
    }
    else
    {
        run = true;
    }
}

int WServerThread::Start()
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
    listener = socket(AF_INET, SOCK_STREAM, 0);

    if (listener == INVALID_SOCKET)
    {
        std::cerr << "Cant create listener: " << WSAGetLastError() << std::endl;
    }

    // Connection informations
    sockaddr_in info;
    info.sin_addr.s_addr = inet_addr("0.0.0.0");
    info.sin_family = AF_INET;
    info.sin_port = htons(20000);

    response = bind(listener, (struct sockaddr*)&info, sizeof(info));

    if (response == SOCKET_ERROR)
    {
        std::cerr << "Cant bind: " << WSAGetLastError() << std::endl;
    }

    return response;
}

WServerThread::~WServerThread()
{
}

void WServerThread::operator()() const
{
    long response;

    // Listen
    response = listen(listener, SOMAXCONN); 

    if (response == SOCKET_ERROR)
    {
        std::cerr << "Cant listen: " << WSAGetLastError() << std::endl;
    }
    else
    {
        std::cout << "Start listening..." << std::endl;
    }

    sockaddr_in clientinfo;
    int size = sizeof(sockaddr_in);

    while (run)
    {
        // get client
        SOCKET client = accept(listener, (struct sockaddr*)&clientinfo, &size);

        char* hostname = inet_ntoa(clientinfo.sin_addr);

        if (client != SOCKET_ERROR)
        {
            std::cout << hostname << " connected" << std::endl;
        }
        else
        {
            std::cerr << "ERROR: " << WSAGetLastError() << std::endl;
        }

        WClientThread clientthread(hostname, (int)client, this);
        std::thread newClient(clientthread);

        newClient.detach();

        m_clients->insert(std::pair<std::string, int>(hostname, client));

        std::cout << "Count of connected clients: " << m_clients->size() << std::endl;
    }
    
    closesocket(listener);

    WSACleanup();

    std::cout << "closed" << std::endl;
}

void WServerThread::Close()
{
    run = false;
}