#include "../../header_files/server/serverThread.hpp"
 
	ServerThread::ServerThread(std::map<std::string, int>* clients) :
     mClients{ clients }
    {

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

	void ServerThread::operator()(std::future<void> futureObj) const
    {
        std::cout << "Listening..." << std::endl;

        // listening
        listen(listener, SOMAXCONN);

        // wait for a connection
        sockaddr_in client;
        socklen_t clientSize = sizeof(client);

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
        }

		Close();
    }

    void ServerThread::Close()
    {
		close(listener);

		mClients->clear();

		std::cout << "Shut down server" << std::endl;
    }