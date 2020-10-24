/******************************************************************************/
/*						Author - Dean Oron									  */
/*						Date - 20.06.20									      */
/*						Reviewer - Ivenna   							      */
/*						Open Lab 86											  */
/******************************************************************************/

#ifndef __ILRD_RD8586_LOGGER_HPP__
#define __ILRD_RD8586_LOGGER_HPP__

#include <string>
#include <boost/chrono.hpp>

#include "singleton.hpp"
#include "waitable_queue.hpp"
#include "priority_queue.hpp"

namespace ilrd
{

class Logger
{
public:
    enum LogLevel 
    {
        ERROR = 0,
        WARNING,
        INFO,
        DEBUG
    };
    void PushMessage(LogLevel log_level, std::string str); // construct the message and push into the queue
    
private:

    struct Message
    {
        Message(LogLevel log_level = ERROR, std::string message = "Default"); // constructor computes time here

        inline bool operator>(const Message& other_) const
        {
            return (m_timepoint > other_.m_timepoint);
        }

        //inline Message& operator=(const Message& other)

        LogLevel m_log_level;
        std::string m_string;
        boost::chrono::system_clock::time_point m_timepoint;
    };

    friend class Singleton<Logger>;

    //thread which will take care of it 
    void PrintMessageIntoLogfile(); // print the message in the logfile with error level and timepoint
    Logger(); 
    ~Logger();

    WaitableQueue<PriorityQueue<Message, std::vector<Message>, std::greater<Message> > , Message> m_wqueue;
    char *m_filename;
    boost::thread m_thread; // to set up in the constructor to run PrintMessageIntoLogfile
};

#define LOG_ERROR(msg) (ilrd::Singleton<ilrd::Logger>::GetInstance()->PushMessage(ilrd::Logger::ERROR, (msg)))
#define LOG_WARNING(msg) (ilrd::Singleton<ilrd::Logger>::GetInstance()->PushMessage(ilrd::Logger::WARNING, (msg)))
#define LOG_INFO(msg) (ilrd::Singleton<ilrd::Logger>::GetInstance()->PushMessage(ilrd::Logger::INFO, (msg)))
#define LOG_DEBUG(msg) (ilrd::Singleton<ilrd::Logger>::GetInstance()->PushMessage(ilrd::Logger::DEBUG, (msg)))
}

#endif /* __ILRD_RD8586_LOGGER_HPP__ */