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
using namespace std;

#include "UDP_Server.h"
/**
 * Initializes the UDP Port on a specified port
 * @param _PORT : Communication port
 * @return
 */
int UDP_Server::InitializeUDPSocket(int _PORT){
    const int BUFFER_SIZE = 1024;
    struct sockaddr_in serverAddr;
    struct sockaddr_in fromAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(_PORT);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    int fromSize = sizeof(fromAddr);

    // Socket
    int udpSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if ( udpSocket < 0 ){
        printf("Socket Failure!");
        exit(EXIT_FAILURE);
    }

    // Bind
    int bindSocket = bind(udpSocket, (sockaddr*)&serverAddr, sizeof(serverAddr));
    if ( bindSocket < 0){
        printf("Bind Failure!");
        exit(EXIT_FAILURE);
    }

    cout << "UDP server connected" << endl;
    cout << "waiting for clients ... " << endl;
    while(true){
        // RECEIVE
        char buffer[BUFFER_SIZE] = {0};
        int recRes = recvfrom(udpSocket, buffer, BUFFER_SIZE, 0, (sockaddr*)&fromAddr, &fromSize);
        if (recRes < 0){
            int closeRes = close(udpSocket);
            if ( closeRes == 0 ) printf("Client Died. Now ");
            break;
        } else if(strcmp(buffer,"break") == 0) {
            puts("Client Disconnected.");
//                break;
        } else if(strcmp(buffer, "shutdown") == 0){
            int closeRes = close(udpSocket);
//                if ( closeRes == 0 ) puts("Client Disconnected.");
            return 0;
        }
        else{
            printf("Client: %s \n", buffer);
        }
        // SEND
        char* confirmMsg = buffer;
        int confirmMsgSize = strlen(confirmMsg);
        int sendRes = sendto(udpSocket, (const char*)confirmMsg, confirmMsgSize, 0, (sockaddr*)&fromAddr, fromSize);
        if(sendRes < 0){
            printf("Send Failure!");
            exit(EXIT_FAILURE);
        }
    }
}