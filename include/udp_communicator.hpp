#ifndef ILRD_RD8586_UDP_COMMUNICATOR_HPP
#define ILRD_RD8586_UDP_COMMUNICATOR_HPP

#include <iostream>
#include <list>
#include <vector>

#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>
#include <boost/bind.hpp>

#include "reactor.hpp"
#include "udp_connector.hpp"
#include "master_storage_manager.hpp"
#include "protocol.hpp"
#include "minion_info.hpp"

namespace ilrd
{

    class IMasterCommunicator
    {
    };

    class UDPcommunicator : IMasterCommunicator
    {
    public:
    UDPcommunicator(const Reactor &reactor, const MasterStorageManager &storage): 
        m_reactor(reactor), m_storage_manager(storage), m_sending_thread(boost::bind(&SendBroadCast, this){}
private:
    
    ///////////nested class
    class UDPMinion : public MinionInfo
    {
        public:
            UDPMinion(int port, const std::string &ip) : MinionInfo(port, ip)
            {
                if ((m_socket = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
                {
                    perror("socket creation failed");
                    exit(EXIT_FAILURE);
                }
            }
            void Send();
            virtual int GetSocket()
            {
                return m_socket;
            }

        private:
            int m_socket;
    };
    //////////////

   //////////nested class
   class MinionUDPIdentifier
   {
        public:
            void SendBroadCast();
   };
    /////////
    UDPConnector m_udp_connector;
    Reactor &m_reactor;
    std::vector<Request> m_requests;
    MasterStorageManager &m_storage_manager;
    std::map<std::string, boost::shared_ptr<UDPMinion> > m_udp_minions;
    boost::thread_t m_sending_thread;
    };
} // namespace ilrd

#endif //ILRD_RD8586_UDP_COMMUNICATOR_HPP
