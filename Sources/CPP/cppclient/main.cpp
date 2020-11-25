
#include <stdio.h>
#include <iostream>
#include "UDP_Client.h"
#include "TCP_Client.h"

using namespace std;

int main(int _argc, char *_argv[]) {

    if ( _argc < 3){
        printf("Some arguments are messing!\n");
        exit(EXIT_FAILURE);
    }

    int PORT = atoi(_argv[1]);
    char* IP = _argv[2];

    printf("Client is talking to server %s on port %d \n", IP, PORT);

    puts("C++ Client Menu:");
    puts("-----------------------------");
    puts("1. Talk to TCP v4 Server");
    puts("2. Talk to TCP v6 Server");
    puts("3. Talk to UDP Echo Server");
    puts("4. EXIT");
    puts("-----------------------------");
    puts("Choose client type to start:");

    int inputInt;
    int q = 1;
    while(q){
        scanf("%d", &inputInt);
        TCP_Client myClient;
        switch (inputInt) {
            case 1: {
                myClient.InitializeTCPClient(4, PORT, IP);
                q = 0;
                break;
            }
            case 2: {
                myClient.InitializeTCPClient(6, PORT, IP);
                q = 0;
                break;
            }
            case 3: {
                UDP_Client myUDPClient;
                int rVal = myUDPClient.InitializeUDPClient(PORT, IP);
                q = 0;
                if (rVal == 0) {
                    return 0;
                }
                break;
            }
            case 4: {
                printf("Exiting..");
                return 0;
            }
            default:
                puts("Bad input, please try again: ");
                break;
        }
    }
}
