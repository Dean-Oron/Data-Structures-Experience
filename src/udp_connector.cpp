////////////////////////////////////////////////////////////////////////////////
// File Name: udp_connector.cpp                                               //
// Author: Dean Oron                                                          //
// Date 06/08/2020                                                            //
// Purpose: functions file for the udp_connector class                        //
// Code Reviewer: Yehuda Levavi                                               //
////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <sys/types.h> 
#include <sys/socket.h> 
#include <string.h>

#include "udp_connector.hpp"
#include "logger.hpp"

using namespace std;
using namespace ilrd;

UDPConnector::UDPConnector(int port)
{
    m_fd = socket(AF_INET, SOCK_DGRAM, 0);

    if (0 > m_fd)
    {
        throw runtime_error("Socket creation failed\n");
    }

    struct sockaddr_in server_addr;

    memset(&server_addr, 0, sizeof(server_addr));

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htons(INADDR_ANY);
    server_addr.sin_port = htons(port);

     if (0 > bind(m_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)))
     {
         LOG_ERROR("Bind to socket failed");
         throw runtime_error("Bidning has failed\n");
     }
}