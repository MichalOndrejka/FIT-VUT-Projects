#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <netdb.h>
#include <getopt.h>
#include <stdbool.h>

#define BUF_SIZE 1024

int type;
bool isInitializedTCP = false;
int client_socket = -1;
int bytes_tx;
int bytes_rx;
char hello[] = "HELLO\n";
char bye[] = "BYE\n";
int flags = 0;
char buffer[BUF_SIZE] = "";

//Function prints error message, closes socket and exits program. To be called when using UDP
void errorUDP(char message[]) {
    fprintf(stderr, "ERROR: %s\n", message);
    close(client_socket);
    exit(EXIT_FAILURE);
}

//Function shutdowns client_socket
void shutDownTCP() {
    shutdown(client_socket, SHUT_RD);
    shutdown(client_socket, SHUT_WR);
    shutdown(client_socket, SHUT_RDWR);
}

//Function prints error message, shutdowns socket, closes socket and exits program. To be called when using TCP
void errorTCP(char message[]) {
    fprintf(stderr, "ERROR: %s\n",message);
    shutDownTCP();
    close(client_socket);
    exit(EXIT_FAILURE);
}

//Handle Ctrl + C interrupt
void cCinterrupt() {
    if (isInitializedTCP == true) {
        printf("\n");
        bytes_tx = send(client_socket, bye, strlen(bye), flags);
        if (bytes_tx < 0) {
            errorTCP("send not succesful");
        }
        printf("%s", bye);

        bytes_rx = recv(client_socket, buffer, BUF_SIZE, flags); //RECIEVE BYE
        if (bytes_rx < 0) {
            errorTCP("recv not succesful");
        }
        if (strcmp(buffer, bye) != 0) {
            errorTCP("didn't recv bye from server");
        }
        printf("%s", buffer);
    } else {
        printf("\n");
    }
    if (type == SOCK_STREAM) {
        shutDownTCP();
    }
    close(client_socket);
    exit(EXIT_SUCCESS);
}

int main(int argc, char **argv) {
    //Check number of argument
    if (argc != 7) {
        fprintf(stderr, "Usage: %s -h <host> -p <port> -m <mode>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    char option;
    char *host = NULL, *mode = NULL;
    int port = -1;

    //Get values of arguments
    while ((option = getopt(argc, argv, "h:p:m:")) != -1) {
        switch (option) {
        case 'h':
            host = optarg;
            break;
        case 'p':
            port = atoi(optarg);
            break;
        case 'm':
            mode = optarg;
            break;
        default:
            fprintf(stderr, "Usage: %s -h <host> -p <port> -m <mode>\n", argv[0]);
            exit(EXIT_FAILURE);
            break;
        }
    }

    //Check if all arguments were provided
    if (host == NULL || port == -1 || mode == NULL) {
        fprintf(stderr, "ERROR: not all arguments provided\n");
        exit(EXIT_FAILURE);
    }

    //Set type based on value of argument mode
    
    if (strcmp(mode, "udp") == 0) {
        type = SOCK_DGRAM;
    } else if (strcmp(mode, "tcp") == 0) {
        type = SOCK_STREAM;
    } else {
        fprintf(stderr, "ERROR: invalid mode: %s\n", mode);
        exit(EXIT_FAILURE);
    }
    //CREATE SOCKET
    int family = AF_INET;

    client_socket = socket(family, type, 0);
    if (client_socket <= 0) {
        fprintf(stderr, "ERROR: socket\n");
        exit(EXIT_FAILURE);
    }

    //GET ADDRESS OF SERVER
    struct hostent *server = gethostbyname(host);
    if (server == NULL) {
        fprintf(stderr, "ERROR: no such host %s\n", host);
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in server_address;
    memset(&server_address, 0, sizeof(server_address));

    server_address.sin_family = family;
    server_address.sin_port = htons(port);
    memcpy(&server_address.sin_addr.s_addr, server->h_addr_list[0], server->h_length);

    struct sockaddr *address = (struct sockaddr *) &server_address;
    socklen_t address_size = sizeof(server_address);

    //Ctrl + C interrupt handler
    signal(SIGINT, cCinterrupt);
    
    //IF UDP
    if (type == SOCK_DGRAM) {
        char binaryMessage[BUF_SIZE + 2] = "";
        int binaryLen = 0;
        while (fgets(buffer, sizeof(buffer), stdin) != NULL) {
            binaryMessage[0] = 0; //OPCODE = 0
            binaryMessage[1] = strlen(buffer); //PAYLOAD LENGTH
            binaryLen = strlen(buffer) + 2;
            strcpy(&binaryMessage[2], buffer);

            //SEND DATA
            bytes_tx = sendto(client_socket, binaryMessage, binaryLen, flags, address, address_size);
            if (bytes_tx < 0) {
                errorUDP("sendto not succesful");
            }
            //RECIEVE DATA
            bzero(buffer, BUF_SIZE);
            bzero(binaryMessage, BUF_SIZE + 2);
            bytes_rx = recvfrom(client_socket, buffer, BUF_SIZE, flags, address, &address_size);
            if (bytes_rx < 0) {
                errorUDP("recvfrom not succesful");
            }
            //PRINT DATA
            if (buffer[1] == 1) {
                printf("ERR:%s\n", &buffer[3]);
            } else {
                printf("OK:%s\n", &buffer[3]);
            }
            bzero(buffer, BUF_SIZE);
            bzero(binaryMessage, BUF_SIZE + 2);
        }
        //CLOSE UDP CONNECTION
        close(client_socket);
    //IF TCP
    } else if (type == SOCK_STREAM) {
        //CONNECT
        if (connect(client_socket, address, address_size) != 0) {
            fprintf(stderr, "ERROR: connect");
            exit(EXIT_FAILURE);
        }

        fgets(buffer, sizeof(buffer), stdin);
        //SEND DATA
        bytes_tx = send(client_socket, buffer, strlen(buffer), flags);
        if (bytes_tx < 0) {
            errorTCP("send not succesful");
        }

        //CHECK IF CLIENT TYPED BYE
        if (strcmp(buffer, hello) != 0) {
            errorTCP("Initiate communication with \"HELLO\" first");
        }

        //RECIEVE DATA
        bzero(buffer, BUF_SIZE); 
        bytes_rx = recv(client_socket, buffer, BUF_SIZE, flags);
        if (bytes_rx < 0) {
            errorTCP("recv not succesful");
        }
        if (strcmp(buffer, bye) == 0) {
            errorTCP("connection closed by server");
        }

        isInitializedTCP = true; //TCP is now initialized

        printf("%s", buffer);
        bzero(buffer, BUF_SIZE); 

        while (fgets(buffer, sizeof(buffer), stdin) != NULL) {
            //SEND DATA
            bytes_tx = send(client_socket, buffer, strlen(buffer), flags);
            if (bytes_tx < 0) {
                errorTCP("send not succesful");
            }

            //CHECK IF CLIENT TYPED BYE
            if (strcmp(buffer, bye) == 0) {
                break;
            }

            //RECIEVE DATA
            bzero(buffer, BUF_SIZE); 
            bytes_rx = recv(client_socket, buffer, BUF_SIZE, flags);
            if (bytes_rx < 0) {
                errorTCP("recv not succesful");
            }
            if (strcmp(buffer, bye) == 0) {
                errorTCP("connection closed by server");
            }
            //PRINT DATA
            printf("%s", buffer);
            bzero(buffer, BUF_SIZE); 
        }

        bzero(buffer, BUF_SIZE); 
        bytes_rx = recv(client_socket, buffer, BUF_SIZE, flags); //Wait for server to send BYE
        if (bytes_rx < 0) {
            errorTCP("recv not succesful");
        }
        if (strcmp(buffer, bye) != 0) {
            errorTCP("didn't recv bye from server");
        }
        printf("%s", buffer); //Print server's last response (BYE)

        //CLOSE TCP CONNECTION
        shutDownTCP(&client_socket);
    }
}
