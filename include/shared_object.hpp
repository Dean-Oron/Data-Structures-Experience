#ifndef __ILRD_RD8586_SHARED_OBJECT_HPP__
#define __ILRD_RD8586_SHARED_OBJECT_HPP__
#include <iostream>
#include<string>
#include <dlfcn.h>

#include "logger.hpp"

namespace ilrd
{

class SharedObject
{
public:
    explicit SharedObject(const std::string& library_path);
    ~SharedObject()
    {
        dlclose(m_handle);
    }

    template <typename T >
    T* LoadSymbol(std::string symbol);

private:
    std::string m_path;
    void * m_handle;
};

template <typename T>
T* SharedObject::LoadSymbol(std::string symbol)
{
    T *to_return = new T;
    to_return = (T *)dlsym(m_handle, symbol.c_str());

    return (to_return);
}

}// ilrd namespace
#endif//__ILRD_RD8586_SHARED_OBJECT_HPP__