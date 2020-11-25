//
// Created by amrel on 10/28/2020.
//
#include <stdio.h>
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <limits>
#include <pthread.h>
#include <windows.h>
#include <winbase.h>
#include <semaphore.h>
#include <map>
#include <string>
#include <vector>
#include <iomanip>
#include <ctime>

using namespace std;

#include "TCP_Server.h"
/**
 * mSemMax : semaphore value
 */
const int mMAXCLIENTS = 3;
int mSemMax = mMAXCLIENTS;

map<string, vector<int>> TCP_Server::ConstructNewData(){
    vector<vector<int>> valuesArray;
    map<string, vector<int>> sensors;
    vector<int> temp;
    srand((unsigned)time(NULL));
    for (int i =0; i < 3; i++){
        int sensorValue = rand() % 1000;
        temp.push_back(sensorValue);
    }
    sensors["air"] = temp;
    temp.clear();

    int sensorValue = rand() % 1000;
    temp.push_back(sensorValue);
    sensors["noise"] = temp;
    temp.clear();

    sensorValue = rand() % 1000;
    temp.push_back(sensorValue);
    sensors["light"] = temp;
    temp.clear();
//        srand((unsigned)time(NULL));
//    for(int j=0;j<3;j++){
//        int tempSize = rand() % 4 + 1; //1 to 4
//        vector<int> temp;
//        for (int i =0; i < tempSize; i++){
//            int sensorValue = rand() % 1000;
//            temp.push_back(sensorValue);
//        }
//        valuesArray.push_back(temp);
//    }
//    sensors["light"] = valuesArray[0];
//    sensors["noise"] = valuesArray[1];
//    sensors["air"] = valuesArray[2];

    return sensors;
}

/**
 * prints the sensors data for debugging reasons
 * @param _sensors : data to print
 */
void TCP_Server::PrintData(map<string, vector<int>> _sensors){
    for ( auto item : _sensors ){
        cout << item.first << ": ";
        for ( auto m : item.second){
            cout << m << " ";
        }
        puts("");
    }
}
/**
 * gets the current timestamp
 * @return string of current timestamp
 */
string TCP_Server::getTimeStamp(){
    time_t now = time(0);
    char* ts = ctime(&now);
    ts[strlen(ts)-1] = 0;
    return ts;
};
/**
 * constructs the string response of the sensor data <SensorValues>#
 * @param _type : needed sensor
 * @return string of sensor data
 */
string TCP_Server::getSensorString(string _type){
    string res = "";
    map<string, vector<int>> data = ConstructNewData();
    for (auto val : data[_type]){
        char result[50];
        sprintf(result, "%d", val);
        res+= (string)result + ";";
    }
    char* fRes = &res[0];
    fRes[strlen(fRes)-1] = 0;
    return string(fRes)+ "#";
}
/**
 * Appends timestamp to the sensor values.
 * @param _type : needed sensor
 * @return char array of <TimeStamp>;<SensorValues>#
 */
char* TCP_Server::getSensor(string _type){
    string res = "";
    string timestamp = getTimeStamp();

    string sensorValues = "|";
    sensorValues += getSensorString(_type);
    res += timestamp + sensorValues;
    char* fRes = &res[0];
    return fRes;
}
/**
 * Gets all sensor data
 * @return
 */
char* TCP_Server::getAllSensors(){
    map<string, vector<int>> data = ConstructNewData();
    string res = "";
    res += getTimeStamp();
    for ( auto item : data ){
        res+= "|" + item.first + ";";
        for ( auto val : item.second){
            char result[50];
            sprintf(result, "%d", val);
            res+= (string)result + ";";
        }
    }
    char* fRes = &res[0];
    fRes[strlen(fRes)-1] = '#';
    return fRes;
}
/**
 * gets all sensor types from the data
 * @return
 */
char* TCP_Server::getSensortypes(){
    map<string, vector<int>> data = ConstructNewData();
    string ret = "";
    for ( auto item : data ){
        ret += item.first + ";";
    }
    char* fRes = &ret[0];
    fRes[strlen(fRes)-1] = '#';
    return fRes;
}
/**
 * releases the semaphore and prints its value
 * @param p_sem
 */
void TCP_Server::IncrCounter(sem_t* p_sem){
    sem_post(p_sem);
    mSemMax++;
    printf("semaphore is: %d \n", mSemMax);
}
/**
 * locks the semaphore and prints its value
 * @param p_sem
 */
void TCP_Server::DecrCounter(sem_t* p_sem){
    sem_wait(p_sem);
    mSemMax--;
    printf("semaphore is: %d \n", mSemMax);
}
/**
 * exits the program and shuts down the server.
 */
void TCP_Server::shutdownServer(){
    puts("Shutting down server.");
    exit(EXIT_SUCCESS);
}

/**
 * Establishes the communication with the client.
 * @param _myParams -> points to the semaphore and the server socket.
 * @return void pointer
 */
void* TCP_Server::ClientCommunication(void* _myParams){

    params* paramsCon = (params*)_myParams;
    const int BUFFER_SIZE = 1024;
    DecrCounter(paramsCon->p_sem);
//    sem_wait(paramsCon->p_sem);
    int commSocket = paramsCon->mSocket;
    while(true){
        char buffer[BUFFER_SIZE];
        memset(buffer, 0, BUFFER_SIZE);
        // RCV
        int recvRes = recv(commSocket, buffer, BUFFER_SIZE, 0);

        if (recvRes < 0){
            int closeRes = close(commSocket);
            if ( closeRes == 0 ) printf("Client Died. Now ");
            break;
        } else if(strcmp(buffer,"break") == 0) {
            int closeRes = close(commSocket);
            if ( closeRes == 0 ) printf("\nClient Disconnected. Now ");
            break;
        } else if(strcmp(buffer, "shutdown") == 0){
            paramsCon->mServer->shutdownServer();
            break;
        }
        else{
            printf("Client: %s \n", buffer);
        }

        // Send
        char* msg;
        if(strcmp(buffer, "getSensortypes()#") == 0){
            msg = getSensortypes();
        } else if(strncmp(buffer, "getSensor()#", 10) == 0) {
            // getSensor(LIGHT | AIR | NOISE)#
//            string bufferCopy = string(buffer);
            string type = "";
            for(int i = 10; i < strlen(buffer) -2; i++ ){
                type += buffer[i];
            }
            char* temp = &type[0];
            msg = getSensor(temp);
        } else if (strcmp(buffer, "getAllSensors()#") == 0){
            msg = getAllSensors();
        } else{
            msg = buffer;
        }

        int msgSize = strlen(msg);
        int sendRes = send(commSocket, msg, msgSize, 0);


        if (sendRes < 0){
            int closeRes = close(commSocket);
            if ( closeRes == 0 ) printf("Client Died. Now ");
            break;
        }
    }
    IncrCounter(paramsCon->p_sem);
//    Release
//sem_post(paramsCon->p_sem);
}
/**
 * Starts the TCP Server
 * @param _VER : IP Version
 * @param _PORT : Communication Port
 * @return
 */
int TCP_Server::InitializeTCPSocket(int _VER, int _PORT){

    int serverSocket, serverBind, commSocket;
    struct sockaddr_in server4Addr;
    struct sockaddr_in6 server6Addr;
    // Socket and Bind
    int bOptVal= 1;
    if(_VER == 4){
        serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        server4Addr.sin_family = AF_INET;
        server4Addr.sin_port = htons(_PORT);
        server4Addr.sin_addr.s_addr = INADDR_ANY;
        serverBind = bind(serverSocket, (sockaddr*)&server4Addr, sizeof(server4Addr));
        if(serverBind < 0){
            printf("bind Failure!");
            exit(EXIT_FAILURE);
        }
    }
    else if (_VER == 6){
        serverSocket = socket(AF_INET6, SOCK_STREAM, IPPROTO_TCP);
        int sockopRes = setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &bOptVal, sizeof(bOptVal));
        if (sockopRes < 0){
            printf("sockopt Failure!");
            exit(EXIT_FAILURE);
        }
        server6Addr.sin6_family = AF_INET6;
        server6Addr.sin6_port = htons(_PORT);
        server6Addr.sin6_addr = IN6ADDR_ANY_INIT;
        serverBind = bind(serverSocket, (sockaddr*)&server6Addr, sizeof(server6Addr));
        if(serverBind < 0){
            printf("bind Failure!");
            exit(EXIT_FAILURE);
        }
    }
    sem_init(&semaphore, 0, mMAXCLIENTS);
    if (serverSocket < 0) {
        printf("Socket failure!");
        exit(EXIT_FAILURE);
    }

    if (serverBind < 0){
        fprintf(stderr, "Bind failure because: %s", strerror(errno));
        exit(EXIT_FAILURE);
    }

    // Listen
    int serverListen = listen(serverSocket, 5);

    if (serverListen < 0 ){
        printf("Listen Failure!");
        exit(EXIT_FAILURE);
    }

    printf("TCP v%d server connected\n", _VER);

    // Accept
    while(true){
        if (mSemMax >= 0){
        cout << "waiting for clients ... " << endl;
        sockaddr_in clientAddr;
        int clientAddrSize = sizeof(clientAddr);
        commSocket = accept(serverSocket, (sockaddr*)&clientAddr, &clientAddrSize);
            if (commSocket < 0){
                printf("Accept Failure!");
                exit(EXIT_FAILURE);
            } else{
                printf("new client connected!\n");

                pthread_t commThread;
                params *myParams = new params();
                myParams -> mSocket = commSocket;
                myParams -> p_sem = &semaphore;
                myParams -> mServer = this;
                pthread_create(&commThread, NULL, ClientCommunication, myParams);

            }

        }

    }
}