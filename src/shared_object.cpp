#include <iostream>

#include "shared_object.hpp"

namespace ilrd
{

SharedObject::SharedObject(const std::string& library_path)
            :m_path(library_path), m_handle(dlopen(m_path.c_str(), RTLD_LAZY))
            {
                LOG_INFO("Was in shared object constructor");
                std::cout << "path is :" << m_path << std::endl;
            }
}