// Server side C/C++ program to demonstrate Socket programming
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/types.h>

#define PORT 8081
#define MAIN_PARAMETER_1 1

int SensFrmCount = 0;
uint8_t value = 88;

typedef struct
{
	uint8_t SOF;
	char SensorFrame[4];
	uint8_t Data;
	uint8_t Checksum;
}frame1;

frame1 FrameToSend;
frame1 FTS;

uint8_t HexChecksum(frame1 *DataCRC)
{
	uint8_t ChkSum = 1;
	ChkSum = DataCRC->SOF ^ DataCRC->SensorFrame[0];
	printf("Checksum: %x\n", ChkSum);
	ChkSum = ChkSum ^ DataCRC->SensorFrame[1];
	printf("Checksum: %x\n", ChkSum);
	ChkSum = ChkSum ^ DataCRC->SensorFrame[2];
	printf("Checksum: %x\n", ChkSum);
	ChkSum = ChkSum ^ DataCRC->SensorFrame[3];
	printf("Checksum: %x\n", ChkSum);
	ChkSum = ChkSum ^ DataCRC->Data;
	printf("Checksum: %x\n", ChkSum);
	
	printf("\nData Frame \nSOF: 0x%x \nSensor Frame: 0x%x, 0x%x, 0x%x, 0x%x \nData: %i\n", DataCRC->SOF, DataCRC->SensorFrame[0], DataCRC->SensorFrame[1], DataCRC->SensorFrame[2], DataCRC->SensorFrame[3], DataCRC->Data);
	
	return ChkSum;
}

frame1 SensorFrameCreate(uint8_t frameData)
{
	
	//Defining frame1 values
    FrameToSend.SOF = 0x15;	//0x15
    FrameToSend.SensorFrame[0] = 0x54;	//TIVA
    FrameToSend.SensorFrame[1] = 0x49;	//TIVA
    FrameToSend.SensorFrame[2] = 0x56;	//TIVA
    FrameToSend.SensorFrame[3] = 0x41;	//TIVA
    FrameToSend.Data = frameData;	//User provided data
    FrameToSend.Checksum = HexChecksum(&FrameToSend);
    printf("Checksum: %x\n", FrameToSend.Checksum);
    
    return FrameToSend;
}

int main(void)
{
    //Data Frame Initialization
    //uint8_t argv_copy = atoi(argv[MAIN_PARAMETER_1]);
    uint8_t argv_copy = value;
    FTS = SensorFrameCreate(argv_copy);
    
    //Server Initialization
    int32_t server_fd; 
    int32_t new_socket; 
    struct sockaddr_in address;
    int32_t opt = 1;
    int32_t addrlen = sizeof(address);
    //char buffer[1024] = {0};
    //char *hello = "Hello from server";

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
       
    // Forcefully attaching socket to the port 8080
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                                                  &opt, sizeof(opt)))
    {
        perror("setsockopt");
        return 1;
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );
       
    
    if (bind(server_fd, (struct sockaddr *)&address, 
                                 sizeof(address))<0)
    {
        perror("bind failed");
        return 1;
    }
    if (listen(server_fd, 3) < 0)
    {
        perror("listen");
        return -1;
    }
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, 
                       (socklen_t*)&addrlen))<0)
    {
        perror("accept");
        return 1;
    }
    while(1)
    {
    //read( new_socket , buffer, 1024);
    //printf("%s\n",buffer );
    //send(new_socket , hello , strlen(hello) , 0 );
    send(new_socket , &FTS , sizeof(FTS) , 0 );
    //printf("Hello message sent\n");}
    return 0;
    }
}
