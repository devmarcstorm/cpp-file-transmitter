#include <iostream>
#include <cstdio>
#include <string>
#include <thread>

#include "server/hpp/wServerThread.hpp"
#include "client/hpp/wclient.hpp"

int startServer()
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

    return 0;
}

int main()
{
    std::cout << "MCM File Transmitter V 1.0" << std::endl;
    std::cout << "If you want to host a server press 's'" << std::endl;
    std::cout << "If you want to connect to a server press 'c'" << std::endl;
    std::cout << "If you want to exit '!q' or '!quit'" << std::endl;

    std::string input;
    
    while (input != "!q" && input != "!quit")
    {
        std::cout << "> ";
        
        std::getline(std::cin, input);

        if (input == "s")
        {
            startServer();

            break;
        }
        else if (input == "c")
        {
            WClient client;

            client.start();

            break;
        }
    }

    std::cout << "Close program" << std::endl;

    return 0;
}