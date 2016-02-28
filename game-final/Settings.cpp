// Settings.cpp
//
//
#include "Settings.h"
#include <iostream>
#include <unistd.h>

Settings::Settings(int argc, char** argv)
{
    int opt = 0;
    type = 0;

    while((opt = getopt(argc, argv, "p:a:c")) != -1)
    {
        switch(opt){
            case 'p':
                port = optarg;
                break;
            case 'a':
                ipAddress = optarg;
                break;

            case 'c':
                type = 1;
                break;

            default:
                //won't go here
                break;
        }
    }

    if(type == 0)
    {
        if(port.empty())
            host();
    }
    else
    {
        if(ipAddress.empty() || port.empty())
            client();
    }
}

void Settings::host()
{
    std::cout << "Enter a port to listen on:" << std::endl;
    getline(std::cin, port);
}

void Settings::client()                                      
{
    std::cout << "Enter an IP to connect to:" << std::endl;
    getline(std::cin, ipAddress);

    std::cout << "Enter the server's  port:" << std::endl;     
    getline(std::cin, port);                               
}                                                     
