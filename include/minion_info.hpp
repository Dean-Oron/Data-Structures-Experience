#ifndef ILRD_RD8586_MINION_INFO
#define ILRD_RD8586_MINION_INFO

#include <iostream>

namespace ilrd
{
    class MinionInfo
    {
    public:
        MinionInfo(int port, const std::string &ip) : m_port(port), m_ip(ip)
        {
            static int counter;
            ++counter;
            m_id = counter;
        }

        virtual int GetPort()
        {
            return m_port;
        }

        virtual std::string GetIP()
        {
            return m_ip;
        }

        virtual void Send() = 0;

        void SetStatus(bool status)
        {
            m_status = status;
        }
        bool GetStatus() const
        {
            return m_status;
        }
        int GetID()
        {
            return m_id;
        }

    private:
        int m_id;
        bool m_status;
        int m_port;
        std::string m_ip;
    };
} // namespace ilrd
#endif //ILRD_RD8586_MINION_INFO