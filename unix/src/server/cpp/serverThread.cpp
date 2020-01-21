 #include "../hpp/serverThread.hpp"
 
    ServerThread::ServerThread(std::map<std::string, int>* clients)
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

    int ServerThread::Start()
    {
        // create socket
        listener = socket(AF_INET, SOCK_STREAM, 0);

        if (listener == -1)
        {
            std::cerr << "ERROR: Can't create socket!" << std::endl;
            return -1;
        }

        // bind ip address port
        sockaddr_in sockAddr;
        sockAddr.sin_family = AF_INET;
        sockAddr.sin_port = htons(20000);

        inet_pton(AF_INET, "0.0.0.0", &sockAddr.sin_addr);
        bind(listener, (sockaddr *)&sockAddr, sizeof(sockAddr));

        return 0;
    }

    ServerThread::~ServerThread()
    {
    }

    void ServerThread::operator()() const
    {
        std::cout << "Listening..." << std::endl;

        // listening
        listen(listener, SOMAXCONN);

        // wait for a connection
        sockaddr_in client;
        socklen_t clientSize = sizeof(client);

        while (run)
        {
            // get client
            int clientSocket = accept(s, (sockaddr *)&client, &clientSize);

            char hostname[NI_MAXHOST];
            char port[NI_MAXSERV];

            memset(hostname, 0, NI_MAXHOST);
            memset(port, 0, NI_MAXSERV);

            if (getnameinfo((sockaddr *)&client, sizeof(client), hostname, NI_MAXHOST, port, NI_MAXSERV, 0) == 0)
            {
                std::cout << "Client " << hostname << " connected! Port " << port << std::endl;
            }
            else
            {
                inet_ntop(AF_INET, &client.sin_addr, hostname, NI_MAXHOST);
                std::cout << "Client " << hostname << " connected! Port " << port << std::endl;
            }
            
            ClientThread clientthread(hostname, clientSocket, this);
            std::thread newClient(clientthread);

            newClient.detach();

            m_clients->insert(std::pair<std::string, int>(hostname, clientSocket));

            std::cout << "Count of connected clients: " << m_clients->size() << std::endl;
        }

        std::cout << "Close socket" << std::endl;

        // Close socket
        close(listener);
    }

    void ServerThread::Close()
    {
        run = false;
    }