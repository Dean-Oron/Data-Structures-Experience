/******************************************************************************/
/* 						Master - Minion Communication Protocol          	  */
/*						Author - Dean Oron									  */
/*						Date - 06.08.20 								      */
/*						Reviewer - Ali Hudson    						      */
/*						Open Lab 86											  */
/******************************************************************************/

#ifndef __ILRD_RD8586_MINION_PROTOCOL_HPP__
#define __ILRD_RD8586_MINION_PROTOCOL_HPP__

#include <stdlib.h>     /*  size_t */
#include <stdint.h>    /* unint_64_t */

#include "logger.hpp"

namespace ilrd
{

    #define MAX_BLOCK_SIZE (4096)
    #define NET_BUFFSIZE (offsetof(Request, m_data[0]) + MAX_BLOCK_SIZE)
    #define NET_BUFFSIZE_RESPONSE (offsetof(Response, m_data[0]) + MAX_BLOCK_SIZE)

    struct Request
    {
        inline size_t RequestSize() const
        {
            return sizeof(*this);
        }

        char m_mode;
        uint64_t m_uid;
        uint64_t m_index;
        char m_data[1];
    }__attribute__((packed));

    struct Response
    {
        inline size_t ResponseSize() const
        {
            return sizeof(*this);
        }

        char m_mode;
        uint64_t m_uid;
        char m_status;
        char m_data[1];
    }__attribute__((packed));

   // #pragma pack(pop)
} // namespace ilrd

#endif /* __ILRD_RD8586_STORAGE_HPP__ */