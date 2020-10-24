////////////////////////////////////////////////////////////////////////////////
// File Name: udp_connector.hpp                                               //
// Author: Dean Oron                                                          //
// Date 06/08/2020                                                            //
// Purpose: header file for the udp_connector class                           //
// Code Reviewer: Yehuda Levavi                                               //
////////////////////////////////////////////////////////////////////////////////

#ifndef ILRD_RD8586_UDP_CONNECTOR_HPP
#define ILRD_RD8586_UDP_CONNECTOR_HPP

#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 

#include "boost/core/noncopyable.hpp"

namespace ilrd
{

class UDPConnector : private boost::noncopyable
{
public:
    explicit UDPConnector(int port);

    ~UDPConnector()
    {
        close(m_fd);
    }

    inline int GetFD()const
    {
        return (m_fd);
    }

private:
    int m_fd;
};

} // namespace ilrd

#endif // ILRD_RD8586_UDP_CONNECTOR_HPP  