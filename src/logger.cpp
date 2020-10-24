#include <iostream>
#include <string>
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <boost/chrono.hpp>
#include <fstream>
#include <time.h>

#include "logger.hpp"

using namespace ilrd;

Logger::Message::Message(LogLevel log_level, std::string message):
    m_log_level(log_level), m_string(message), m_timepoint(boost::chrono::system_clock::now())
{
}

Logger::Logger():m_filename(getenv("LOGGERNAME")), 
                m_thread(boost::bind(&Logger::PrintMessageIntoLogfile, this))
{
    std::fstream fs;
    fs.open(m_filename, ios::app);
    fs.close();
}

Logger::~Logger()
{   
    while(!m_wqueue.Empty())
    ;  

    m_thread.interrupt();
    m_thread.join();
}

void Logger::PushMessage(LogLevel log_level, std::string str)
{
    Message msg(log_level, str);
    m_wqueue.Push(msg);
}

void Logger::PrintMessageIntoLogfile()
{
    Logger::Message msg(ERROR, "Default");
    string total_msg;
    string log_level;
    WaitableQueue<PriorityQueue<Message, std::vector<Message>, std::greater<Message> > , Message>::Milisec ms(1000);
    while(1)
    {   
        m_wqueue.Pop(msg);
        switch (msg.m_log_level)
        {
        case (0):
            log_level = "ERROR: ";
            break;
        case (1):
            log_level = "WARNING: ";
            break;

        case (2):
            log_level = "INFO: ";
            break;
        case (3):
            log_level = "DEBUG: ";
            break;
        } 

        std::time_t t = boost::chrono::system_clock::to_time_t(msg.m_timepoint);
        total_msg = std::ctime(&t);

        total_msg += log_level + msg.m_string + "\n\n"; 
        
        std::fstream fs;
        fs.open("logger.txt", std::fstream::app);
        fs.write(total_msg.c_str(), total_msg.length()); 
        fs.close();
    }
}
