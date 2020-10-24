#ifndef __ILRD_RD8586_SMALL_LOGGER_HPP__
#define __ILRD_RD8586_SMALL_LOGGER_HPP__

#include <iostream>
#include <fstream>

using namespace std;
namespace ilrd
{
class Logger
{
public:
    Logger() 
    {
        cout << "creating logger" << endl;
    }
    
    ~Logger() {};

    inline void OpenLogFile(string file_name);
    inline void WriteToLogFile(const string& content);
    inline void CloseLogFile();

private:
    ofstream m_file_name;
};

inline void Logger::OpenLogFile(string file_name)
{
    m_file_name.open(file_name,  std::fstream::app);
}

inline void Logger::WriteToLogFile(const string& content)
{
    m_file_name << content << endl;
}

inline void Logger::CloseLogFile()
{
    m_file_name.close();
}
}// ilrd namespace

#endif