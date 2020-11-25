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

#include "UDP_Client.h"
/**
 * Starts the UDP Server based on the specified parameters.
 * @param _PORT : Connection Port.
 * @param _IP Connection IP.
 * @return 0 when the server shuts down.
 */
int UDP_Client::InitializeUDPClient(int _PORT, char* _IP){
    const int BUFFER_SIZE = 1024;
    sockaddr_in toAddr;
    toAddr.sin_family = AF_INET;
    toAddr.sin_port = htons(_PORT);
    toAddr.sin_addr.s_addr = inet_addr(_IP);
    memset(&(toAddr.sin_zero), '\0',8);
    int toSize = sizeof(toAddr);

    int udpSocket = socket(AF_INET, SOCK_DGRAM, 0);
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
        int sendRes = sendto(udpSocket, msg, msgSize, 0, (sockaddr*)&toAddr, toSize);
        if (sendRes == 0) {
            printf("Partner Disconnected in sending!\n");
            exit(EXIT_FAILURE);
        } else if (sendRes == msgSize) {
            printf("All data is transferred!\n");
        } else if (sendRes < 0) {
            printf("Error encountered in sending!\n");
            exit(EXIT_FAILURE);
        }

        if (strcmp(msg, "break") == 0) {break;}
        if(strcmp(msg, "shutdown") == 0){
            int closeRes = close(udpSocket);
            printf("Closing socket: %d\n", closeRes);
            return 0;
        }

        int recRes = recvfrom(udpSocket, (char *) buffer, BUFFER_SIZE, 0, (sockaddr *) &toAddr, &toSize);
        if (recRes == 0) {
            printf("Partner Disconnected in receiving!\n");
            exit(EXIT_FAILURE);
        } else if (recRes < 0) {
            printf("Error encountered in receiving!\n");
            exit(EXIT_FAILURE);
        } else {
            printf("ECHO: %s\n", buffer);
        }

    }
}
