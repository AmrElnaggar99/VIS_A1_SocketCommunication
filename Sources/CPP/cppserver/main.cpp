
#include <stdio.h>
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "UDP_Server.h"
#include "TCP_Server.h"
#define WIN32_LEAN_AND_MEAN


using namespace  std;

int main(int argc, char *argv[]) {

    if ( argc < 2){
        printf("Some arguments are messing!\n");
        exit(EXIT_FAILURE);
    }

    int PORT = atoi(argv[1]);

    printf("Server is connected on port %d \n", PORT);

    puts("C++ Server Menu:");
    puts("-----------------------------");
    puts("1. Start TCP v4 Server");
    puts("2. Start TCP v6 Server");
    puts("3. Start UDP Echo Server");
    puts("4. EXIT");
    puts("-----------------------------");
    puts("Choose server type to start:");

    int inputInt;
    int q = 1;
    int rVal;
    while(q){
        scanf("%d", &inputInt);
        TCP_Server myServer;
        switch (inputInt) {
            case 1:
                rVal = myServer.InitializeTCPSocket(4, PORT);
                q = 0;
                break;
            case 2:
                rVal = myServer.InitializeTCPSocket(6, PORT);
                q = 0;
                break;
            case 3:
                UDP_Server my_UDP_Server;
                rVal = my_UDP_Server.InitializeUDPSocket(PORT);
                q = 0;
                break;
            case 4:
                printf("Exiting..");
                return 0;

            default:
                puts("Bad input, please try again: ");
                break;
        }
        if (rVal == 0){
            puts("Shutting down the server.");
            return 0;
        }
    }

    return 0;

}
