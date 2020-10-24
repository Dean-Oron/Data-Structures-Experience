////////////////////////////////////////////////////////////////////////////////
// File Name: udp_connector.cpp                                               //
// Author: Dean Oron                                                          //
// Date 06/08/2020                                                            //
// Purpose: test file for the udp_connector class                             //
// Code Reviewer: Yehuda Levavi                                               //
////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <string.h>

#include "udp_connector.hpp"

using namespace std;
using namespace ilrd;
#define BUFFER_SIZE (20)
int main()
{
    UDPConnector udp(8080);

    socklen_t length;
    int bytes_received;
    char buffer[BUFFER_SIZE]; 
    char message[] = "I'm Alive";
    struct sockaddr_in server_addr;
    std::cout << "socket value is: " << udp.GetFD() << std::endl;

    server_addr.sin_family = AF_INET; 
    server_addr.sin_addr.s_addr = INADDR_ANY; 
    server_addr.sin_port = htons(8080);

    while(1)
    {
        sleep(1);
        sendto(udp.GetFD(), (const char *)message, strlen(message),  
            MSG_CONFIRM, (const struct sockaddr *) &server_addr, 
                sizeof(server_addr));  
    
        bytes_received = recvfrom(udp.GetFD(), (char *)buffer, BUFFER_SIZE,  
                    MSG_WAITALL, ( struct sockaddr *) &server_addr, 
                    &length); 
        buffer[bytes_received] = '\0'; 
        std::cout << "Server: " << buffer << std::endl;
    }
    
    return 0;
}