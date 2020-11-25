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

#include "TCP_Client.h"
/**
 * Starts the TCP server based on specified parameters.
 * @param _VER : IP version (4 or 6)
 * @param _PORT : Connection Port
 * @param _IP : Connection IP
 */
void TCP_Client::InitializeTCPClient(int _VER, int _PORT, char* _IP){
    const int BUFFER_SIZE = 1024;
    int commSocket, connectServer;
    if (_VER == 4) {
        puts("TCP v4 Started.");
        commSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        sockaddr_in serverAddr;
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = htons(_PORT);
        serverAddr.sin_addr.s_addr = inet_addr(_IP);
        connectServer = connect(commSocket, (sockaddr*)&serverAddr, sizeof(serverAddr));
    }
    else if ( _VER == 6){
        puts("TCP v6 Started.");
        commSocket = socket(AF_INET6, SOCK_STREAM, IPPROTO_TCP);
        sockaddr_in6 serverAddr;
        serverAddr.sin6_family = AF_INET6;
        serverAddr.sin6_port = htons(_PORT);
        if (inet_pton(AF_INET6, _IP, &(serverAddr.sin6_addr)) < 1){
            printf("inet_pton error");
            exit(EXIT_FAILURE);
        }
        connectServer = connect(commSocket, (sockaddr*)&serverAddr, sizeof(serverAddr));
    }

    if ( connectServer < 0) {
        printf("Connect Failure!");
        exit(EXIT_FAILURE);
    }
    // send
    int q = 1;
    while (true) {
        printf("Write something to send: \n");
        char buffer[BUFFER_SIZE] = {0};
        char msg[BUFFER_SIZE];
        // First input reading has an empty line for some reason, so we ignore it from the buffer stream only the first time.
        if (q) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            q = 0;
        }
        cin.getline(msg, BUFFER_SIZE);
        int msgSize = strlen(msg);

        int sendRes = send(commSocket, msg, msgSize, 0);

        if (sendRes == 0) {
            printf("Partner Disconnected in sending!\n");
            exit(EXIT_FAILURE);
        } else if (sendRes == msgSize) {
            printf("All data is transferred!\n");
        } else if (sendRes < 0) {
            printf("Error encountered in sending!\n");
            exit(EXIT_FAILURE);
        }
        // RECV
        int recvRes = recv(commSocket, buffer, BUFFER_SIZE, 0);

        if (recvRes == 0) {
            printf("Partner Disconnected in receiving!\n");
            exit(EXIT_FAILURE);
        } else if (recvRes < 0) {
            printf("Error encountered in receiving!\n");
            exit(EXIT_FAILURE);
        } else {
            printf("ECHO: %s\n", buffer);
        }
        if (strcmp(msg, "break") == 0) break;
        if(strcmp(msg, "shutdown") == 0) break;
    }

    int closeRes = close(commSocket);
    cout << closeRes << endl;

}
