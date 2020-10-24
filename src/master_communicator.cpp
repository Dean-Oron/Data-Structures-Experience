/******************************************************************************/
/* 						Master Communicator Implemention            		  */
/*						Author - Dean Oron									  */
/*						Date - 06.08.20 								      */
/*						Reviewer - Ali Hudson    						      */
/*						Open Lab 86											  */
/******************************************************************************/

#include <iostream>
#include <boost/bind.hpp>

#include "master_communicator.hpp"

using namespace std;
using namespace ilrd;

MasterCommunicator::MasterCommunicator(int port, Reactor& reactor, 
                                                       ActionRequest ar_func)
: m_ar_func(ar_func), m_reactor(reactor), m_con(port), m_port(port), 
        m_callback(boost::bind(&MasterCommunicator::ReadRequest, this, _1)), m_request(NULL)
{
    m_reactor.Add({READ, m_con.GetFD()}, &m_callback); 
}

void MasterCommunicator::ReadRequest(int fd)
{
    (void)fd;
    m_request = static_cast< Request * >(operator new(NET_BUFFSIZE));

    struct sockaddr_storage server_addr;
    socklen_t length = sizeof(sockaddr_storage);
    bzero(&server_addr, sizeof(server_addr));

    memset((void*)m_request, 0, NET_BUFFSIZE);
    
    cout << "net buffsize: " << NET_BUFFSIZE << endl;
     
    int bytes_received = recvfrom(m_con.GetFD(), (char *)m_request, 
          NET_BUFFSIZE, 0, (struct sockaddr*) &server_addr, 
          &length);

    if (bytes_received < 0)
    {
        LOG_ERROR("Failed to read from the buffer in ReadRequest");
    }

        m_client_addr = *(sockaddr_in*)&server_addr;

        cout << "recieve from returns " << bytes_received << endl;

        cout << "fd from READ REQUEST value is " << m_con.GetFD() << endl;
        /* cout << "port num is " << m_ */

    cout << "ARFunc\n";
    cout << m_request->m_mode << std::endl;
    cout << m_request->m_index << std::endl;
    cout << m_request->m_uid << std::endl;
    cout << m_request->m_data << std::endl;

    m_ar_func(*m_request);
}

void ilrd::MasterCommunicator::Reply(const Response& response) const
{
    struct sockaddr_in server_addr;
    bzero(&server_addr, sizeof(sockaddr_in));

    size_t msg_size = 0;

    //was write request
    if (response.m_mode == 1)
    msg_size = offsetof(Response, Response::m_data);

    //was read request
    else
    msg_size = NET_BUFFSIZE_RESPONSE;

    if (0 > sendto(m_con.GetFD(), (const char *)&response, msg_size,  
                  0, (const struct sockaddr *) &m_client_addr, 
                  sizeof(m_client_addr)))
    {
        LOG_ERROR("Faied to send response");
        throw "Failed to send response\n";
    }
}