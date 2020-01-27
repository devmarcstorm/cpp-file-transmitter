#ifndef CLIENT_HPP
#define CLIENT_HPP

#pragma comment(lib, "ws2_32.lib")  // Linker
#include <cstdio>
#include <iostream>
#include <WinSock2.h>
#include <Windows.h>
#include <fstream>
#include <string>
#include <vector>
#include <thread>

#include <chrono>
#include <future>

#include "../Tools.hpp"
#include "../Base64.hpp"

class Client
{
public:

    Client();

    int start(std::string ip);

    void sender(std::string input);

    void close();

    void sendNextFile(std::string ip);

    void sendFile();

    ~Client();

private:

    void readPath();
    void readFile(std::string filename);
    void sendFile(std::string& file, std::string filename);

    SOCKET mSock;

    std::promise<void> mExitSignal;

    bool mIsOK;
    bool mSingleFile;

    std::string mIsDirectory;
    std::string mRemoteIp;
    std::string mMessage;
    std::string mOutput;
    std::string mInput;

    int mOffset;

    std::vector<std::string> mFiles;
    std::vector<std::string>::iterator mFilesIt;
};

#endif