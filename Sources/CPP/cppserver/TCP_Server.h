//
// Created by amrel on 10/28/2020.
//

#ifndef SERVER_TCP_SERVER_H
#define SERVER_TCP_SERVER_H
#include <semaphore.h>
#include <map>
#include <string>
#include <vector>

using namespace std;
class TCP_Server {
private:
    /**
     * semaphore to limit concurrent connections
     */
    sem_t semaphore;
public:
    struct params{
        /**
         * mSocket : The opened socket to communicate on.
         */
        int mSocket;
        /**
         * p_sem : pointer to the semaphore.
         */
        sem_t* p_sem;
        /**
         * mServer : reference to the TCP server, to shut it down.
         */
        TCP_Server *mServer;
    };
    int InitializeTCPSocket(int _VER, int _PORT);
    static map<string, vector<int>> ConstructNewData();
    static void* ClientCommunication(void* commSocket);
    static void IncrCounter(sem_t* p_sem);
    static void DecrCounter(sem_t* p_sem);
    void shutdownServer();
    void PrintData(map<string, vector<int>> _sensors);
    static char* getSensortypes();
    static char* getSensor(string _type);
    static string getTimeStamp();
    static string getSensorString(string _type);
    static char* getAllSensors();
};


#endif //SERVER_TCP_SERVER_H
