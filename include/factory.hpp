/******************************************************************************
 * API FACTORY
 * 02.08.2020
 * Dean Oron
******************************************************************************/

#ifndef ILRD_RD8586_FACTORY_HPP
#define ILRD_RD8586_FACTORY_HPP

#include <iostream>
#include <map>
#include "boost/core/noncopyable.hpp"
#include "logger.hpp"

using namespace std;
namespace ilrd
{
//Factory can be created as a singleton ,Shapes can be used for test
template<class KEY,class BASE,class PARAMS ,class BUILDER = BASE* (*)(PARAMS)>
class Factory:private boost::noncopyable
{
public:

    // Add Function
    // Receives: key - key to the specific function to be created.
    //           creation_func - function which creates a specific object.
    // Exceptions: throws runtime_error if function is NULL;
    // Throws run-time exception if key already exist
    // BUILDER accepts functions which follows this declaration BASE* (*) (PARAMS) 
     
    void Add(KEY key, BUILDER creation_func);

    // Create Function
    // Receives: key - key to the specific function to be created.
    //           build_params - params that are needed for a creation function.
    // Exceptions: throws runtime_error if creation fails or key doesn't exist.

    BASE* Create(KEY key, PARAMS build_params)const;
private:
    std::map<KEY, BUILDER>m_factory_creation;
};

template<class KEY,class BASE,class PARAMS ,class BUILDER>
void Factory<KEY, BASE, PARAMS, BUILDER>::Add(KEY key, BUILDER build_func)
{
    if (!build_func)
    {
        std:: cout << "check1" << endl;
        throw ("build function hasn't been supplied!");
    }

    if (m_factory_creation.find(key) != m_factory_creation.end())
    {
        std:: cout << "check2" << endl;
        LOG_ERROR("Factory: key already exist");
        throw("key already exists");
    }

    else
    {
        m_factory_creation[key] = build_func;
    }
}

template<class KEY,class BASE,class PARAMS ,class BUILDER>
BASE *Factory<KEY, BASE, PARAMS, BUILDER>::Create(KEY key, PARAMS params) const
{
    if (m_factory_creation.find(key) == m_factory_creation.end())
    {   
        LOG_ERROR("Factory: key was not found");
        throw std::runtime_error("key was not found");
    } 

    BASE *to_return = m_factory_creation.find(key)->second(params);

    return (to_return);
} 

} // namespace ilrd

#endif // ILRD_RD8586_FACTORY_HPP 

