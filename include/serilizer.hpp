/******************************************************************************/
/* 						Serializer                  						  */
/*						Author - Dean Oron									  */
/*						Date - 14.07.20 								      */
/*						Reviewer - Yehuda       						      */
/*						Open Lab 86											  */
/******************************************************************************/

#ifndef ILRD_RD8586_SERIALIZER_HPP
#define ILRD_RD8586_SERIALIZER_HPP

#include <boost/bind.hpp>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <typeinfo>

#include "logger.hpp"
#include "factory.hpp"
#include "circle.hpp"

using namespace ilrd;

template <class BASE>
class Serializer : boost::noncopyable
{
public:
    template <class DERIVED>
    void Add(); // DERIVED should have default ctor
    void Serialize(const BASE &obj, std::ostream &os) const;
    boost::shared_ptr<BASE> Deserialize(std::istream &is) const;

private:
    template <class DERIVED>
    static BASE *Creator(std::istream &is); //call the constructor of the object

    Factory<std::string, BASE, std::istream &> m_factory;
};

template <class BASE>
template <class DERIVED>
void Serializer<BASE>::Add()
{
    m_factory.Add(typeid(DERIVED).name(), Creator<DERIVED>);
    LOG_DEBUG(std::string("added key to serializer: ") + typeid(DERIVED).name());
}

template< class BASE >
template < class DERIVED >
BASE *Serializer<BASE>::Creator(std:: istream &is) //call the constructor of the object 
{
    DERIVED *built_obj = new DERIVED;
    *built_obj << is;

    return (built_obj);
}

template < class BASE >
void Serializer< BASE >::Serialize(const BASE& obj, std::ostream& os) const
{
    os << typeid(obj).name() << std::endl;
    LOG_DEBUG(std::string("serializing object of type ") + typeid(obj).name());

    obj >> os;
}

/* template < class BASE >
boost::shared_ptr< BASE > Serializer< BASE >::Deserialize(std::istream& is) const
{
    std::string key = "";
    do
    {
        std::getline(is, key);
    } while (key == "");
    
    LOG_DEBUG(std::string("deserializing key ") + key);

    return (boost::shared_ptr< BASE >(m_factory.Create(key, is)));
} */

template <class BASE>
boost::shared_ptr<BASE> Serializer<BASE>::Deserialize(std::istream &is) const
{
    std::string key;

    std::getline(is, key);
    std::cout << "the key from deserialize is: " << key << std::endl;

    boost::shared_ptr<BASE> ptr(m_factory.Create(key, is));
    return (ptr);
}

#endif /* ILRD_RD8586_SERIALIZER_HPP */