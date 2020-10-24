#include <iostream>
#include <fstream>
#include <pthread.h>
#include <unistd.h>
#include <cstring>

#include "singleton.hpp"
#include "small_logger.hpp"

using namespace std;
using namespace ilrd;

#define NUM_THREADS (5)

void *ThreadFunc(void *arg)
{
    (void)arg;
    Logger *log = Singleton<Logger>::GetInstance();

    log->OpenLogFile("myfile.txt");
    log->WriteToLogFile("trail");
    cout << "End of ThreadFunc" << endl;

    return (NULL);
}

int main()
{
    pthread_t threads[NUM_THREADS];

    for (int i = 0; i < NUM_THREADS; ++i)
    {
        pthread_create(&threads[i], NULL, ThreadFunc, NULL);
    }

    for (int i = 0; i < NUM_THREADS; ++i)
    {
        pthread_join(threads[i], NULL);
    }

    Logger *log = Singleton<Logger>::GetInstance();
    log->CloseLogFile();

    return (0);
}