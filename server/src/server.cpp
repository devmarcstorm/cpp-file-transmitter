#include <iostream>
#include <string>
#include <thread>

#include "../hpp/serverThread.hpp"

int main()
{
    // Create server thread
    ServerThread server;

    if (server.broken)
    {
        return -1;
    }

    std::thread newServer(server);

    newServer.detach();

    // Shut down loop
    std::string input;

    while (true)
    {
        std::cin >> input;

        if (input == "!quit")
        {
            server.Close();
            break;
        }
    }

    std::cout << "Program finished" << std::endl;
    return 0;
}