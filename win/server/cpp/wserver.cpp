#include <iostream>
#include <string>
#include <thread>

#include "../hpp/wServerThread.hpp"

int main()
{
    // Create server thread
    std::map<std::string, int> Clients;

    WServerThread server(&Clients);

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

        if (input == "!q" || input == "!Q" || input == "!Quit" || input == "!quit")
        {
            server.Close();
            break;
        }
    }

    std::cout << "Shut down" << std::endl;
    return 0;
}