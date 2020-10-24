/******************************************************************************/
/* 						Master Communicator client file               		  */
/*						Author - Dean Oron									  */
/*						Date - 06.08.20 								      */
/*						Reviewer - Ali Hudson    						      */
/*						Open Lab 86											  */
/******************************************************************************/

#include <iostream>      
#include <cstring>      

#include <stdio.h>      /* printf */
#include <unistd.h>     /* close , sleep */ 
#include <netinet/in.h> /* socket, htons, sendto, recvfrom */
#include <arpa/inet.h>  /*inet_addr */

#include <stdlib.h>     /* srand rand */
#include <time.h>       /* srand rand */

#include "protocol.hpp"

#define PORT (1234)
#define MSG_SIZE (4096)

using namespace ilrd;
using namespace std;

void EndWithError(char *error_message);  /* External error handling function */
  
int main() 
{ 
    int sock_fd = 0; 
    struct sockaddr_in server_addr = {0}; 
    int length = sizeof(server_addr);

    sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (0 > sock_fd) 
    { 
        EndWithError("socket"); 
    } 

    server_addr.sin_family = AF_INET; 
    server_addr.sin_port = htons(PORT); 
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    size_t msg_size = NET_BUFFSIZE;

    Request *main_request = (Request *)operator new (NET_BUFFSIZE);

    std::cout << "sizeof current request is " << NET_BUFFSIZE << std::endl;

    main_request->m_mode = 1;
    main_request->m_index = 5;
    main_request->m_uid = 111111;
    memcpy(main_request->m_data, "Dean Oron", 10);
      
    char buffer[NET_BUFFSIZE_RESPONSE]; 
    int n_bytes = 0;

    n_bytes = sendto(sock_fd, main_request, NET_BUFFSIZE, 
            0, (const struct sockaddr *) &server_addr, length); 
    if (0 > n_bytes)
    {
        EndWithError("sendto"); 
    }

    printf("Ping\n"); 

    delete main_request;

    n_bytes = recvfrom(sock_fd, buffer, NET_BUFFSIZE, 0, 
                    (struct sockaddr *) &server_addr, (socklen_t*)&length); 
    if (0 > n_bytes)
    {
        EndWithError("sendto"); 
    }
        
    Response *response = (Response*)buffer;
    std::cout << response->m_mode << std::endl;
    std::cout << response->m_status << std::endl;
    std::cout << response->m_uid << std::endl;
    std::cout << response->m_data << std::endl;
  
    close(sock_fd);

    return 0;  
} 

void EndWithError(char *error_message)
{
    perror(error_message);
    exit(1);
}