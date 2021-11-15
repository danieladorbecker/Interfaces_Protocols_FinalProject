#define _GNU_SOURCE
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdint.h>
#include <sys/types.h>
#include <pthread.h>

#define PORT2 8088
#define bufferLen 100

int main(int argc, char  *argv[])
{
    int32_t html_server_fd; 
    int32_t new_html_socket; 
    struct sockaddr_in address;
    int32_t option = 1;
    int32_t addresslen = sizeof(address);
    int value = 3;
    int32_t x = 33;
    
    char *line4 = "TPS Opening Percentage: \r\n\r\n";
    char sendBuffer[bufferLen];
    char *num;

    // Creating socket file descriptor
    if ((html_server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
       
    // Forcefully attaching socket to the port 8080
    if (setsockopt(html_server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                                                  &option, sizeof(option)))
    {
        perror("setsockopt");
        return 1;
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT2 );
       
    
    if (bind(html_server_fd, (struct sockaddr *)&address, 
                                 sizeof(address))<0)
    {
        perror("bind failed");
        return 1;
    }
    if ((x = (listen(html_server_fd, 3)) < 0))
    {
        perror("listen");
        return -1;
    }
    if ((new_html_socket = accept(html_server_fd, (struct sockaddr *)&address, 
                       (socklen_t*)&addresslen))<0)
    {
        perror("accept");
        return 1;
    }
    printf("x %i\n", x);
    if(x == 0)
    {
        line4 = "TPS Opening Percentage: ";
        asprintf(&num, "%d", value);
        strcat(strcpy(sendBuffer, line4), num);
        send(new_html_socket , sendBuffer , strlen(sendBuffer) , 0 );
        x = 1;
        printf("x %i\n", x);
    }
    if ((x = (listen(html_server_fd, 1)) < 0))
    {
        perror("listen");
        return -1;
    }
    printf("Hello message sent\n");
    return 0;
}
