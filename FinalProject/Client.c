// Client side C/C++ program to demonstrate Socket programming
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#define PORT 8081
   
int main(int argc, char const *argv[])
{
    int sock = 0; 
    struct sockaddr_in serv_addr;
    char *hello = "Hello from client";
    char buffer[1024] = {0};
    
    
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("error \n");
        return -1;
    }
   
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
       
    // Convert IPv4 and IPv6 addresses from text to binary form
    //if(inet_pton(AF_INET, "192.168.43.180", &serv_addr.sin_addr)<=0)	//Dani
    if(inet_pton(AF_INET, "192.168.100.95", &serv_addr.sin_addr)<=0)	//Dani Home
    {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }
   
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\nConnection Failed \n");
        return -1;
    }
   
    while(1)
    {
        send(sock, hello, strlen(hello), 0);
        printf("Hello message sent\n");
        
        read( sock , buffer, 1024);
        printf("%s\n",buffer);
       
        printf("\nDATA FRAME\n");
        printf("SOF: 0x%x\n", buffer[0]);
        printf("Device Address: 0x%x 0x%x 0x%x 0x%x\n", buffer[1], buffer[2], buffer[3], buffer[4]);
        printf("Data: %i\n", buffer[5]);
        printf("Checksum: %x\n", buffer[6]);
    }
	    
    return 0;
}
