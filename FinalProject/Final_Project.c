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

#define SLAVE_ADDRESS_TI 0x10
#define SLAVE_ADDRESS_LCD 0x3C
#define PORT 8081
#define MAIN_PARAMETER_1 1
#define PORT2 8088
#define bufferLen 100

int fd_LCD;
int fd_TI;
unsigned char LCD_screen_buffer[1024];
char LCD_line1[18];
int value;
uint8_t newValue;

int SensFrmCount = 0;

int *MAIN_PARAMETER_THREAD = (int*)1;

pthread_t tid[2];
int counter;
pthread_mutex_t lock;


void *threadServer(void *param);
void *threadI2C(void *param);
void *threadInitLCD(void *param);
void *threadHTML(void *param);
    
typedef struct
{
	uint8_t SOF;
	char SensorFrame[4];
	uint8_t Data;
	uint8_t Checksum;
}frame1;

frame1 FrameToSend;
frame1 FTS;

void LCD_parse (char* letra, unsigned char caracter)
{
    switch (caracter)
    {
        //Números y guión
        case '0':
            letra[0]=0x00;letra[1]=0x00;letra[2]=0xfe;letra[3]=0x82;letra[4]=0xFe;letra[5]=0x00;letra[6]=0x00;
            break;
        case '1':
            letra[0]=0x00;letra[1]=0x00;letra[2]=0x84;letra[3]=0xfe;letra[4]=0x80;letra[5]=0x00;letra[6]=0x00;
            break;
        case '2':
            letra[0]=0x00;letra[1]=0x00;letra[2]=0xe4;letra[3]=0x92;letra[4]=0x8c;letra[5]=0x00;letra[6]=0x00;
            break;
        case '3':
            letra[0]=0x00;letra[1]=0x00;letra[2]=0x92;letra[3]=0x92;letra[4]=0xFe;letra[5]=0x00;letra[6]=0x00;
            break;
        case '4':
            letra[0]=0x00;letra[1]=0x00;letra[2]=0x1e;letra[3]=0x10;letra[4]=0xFe;letra[5]=0x00;letra[6]=0x00;
            break;
        case '5':
            letra[0]=0x00;letra[1]=0x00;letra[2]=0x9e;letra[3]=0x92;letra[4]=0xF2;letra[5]=0x00;letra[6]=0x00;
            break;
        case '6':
            letra[0]=0x00;letra[1]=0x00;letra[2]=0xfe;letra[3]=0x92;letra[4]=0xF2;letra[5]=0x00;letra[6]=0x00;
            break;
        case '7':
            letra[0]=0x00;letra[1]=0x0;letra[2]=0x02;letra[3]=0x02;letra[4]=0xFE;letra[5]=0x00;letra[6]=0x00;
            break;
        case '8':
            letra[0]=0x00;letra[1]=0x00;letra[2]=0xfe;letra[3]=0x92;letra[4]=0xFe;letra[5]=0x00;letra[6]=0x00;
            break;
        case '9':
            letra[0]=0x00;letra[1]=0x00;letra[2]=0x1e;letra[3]=0x12;letra[4]=0xFe;letra[5]=0x00;letra[6]=0x00;
            break;
        case '-':
            letra[0]=0x00;letra[1]=0x18;letra[2]=0x18;letra[3]=0x18;letra[4]=0x18;letra[5]=0x18;letra[6]=0x00;
            break;

        //Minúsculas
        case 'a':
            letra[0]=0x00;letra[1]=0x70;letra[2]=0x88;letra[3]=0x88;letra[4]=0xF0;letra[5]=0x80;letra[6]=0x00;
            break;

        case 'b':
            letra[0]=0x00;letra[1]=0x00;letra[2]=0xFE;letra[3]=0x90;letra[4]=0xF0;letra[5]=0x00;letra[6]=0x00;
            break;

        case 'c':
            letra[0]=0x00;letra[1]=0x00;letra[2]=0xF8;letra[3]=0x88;letra[4]=0x88;letra[5]=0x00;letra[6]=0x00;
            break;

        case 'd':
            letra[0]=0x00;letra[1]=0x00;letra[2]=0xF0;letra[3]=0x90;letra[4]=0xFE;letra[5]=0x00;letra[6]=0x00;
            break;

        case 'e':
            letra[0]=0x00;letra[1]=0x00;letra[2]=0xF8;letra[3]=0xA8;letra[4]=0xB8;letra[5]=0x00;letra[6]=0x00;
            break;

        case 'f':
            letra[0]=0x00;letra[1]=0x00;letra[2]=0xFE;letra[3]=0x32;letra[4]=0x00;letra[5]=0x00;letra[6]=0x00;
            break;

        case 'g':
            letra[0]=0x00;letra[1]=0x00;letra[2]=0xDE;letra[3]=0x92;letra[4]=0xFE;letra[5]=0x00;letra[6]=0x00;
            break;

        case 'h':
            letra[0]=0x00;letra[1]=0x00;letra[2]=0xFE;letra[3]=0x10;letra[4]=0xF0;letra[5]=0x00;letra[6]=0x00;
            break;

        case 'i':
            letra[0]=0x00;letra[1]=0x00;letra[2]=0x00;letra[3]=0xFA;letra[4]=0x00;letra[5]=0x00;letra[6]=0x00;
            break;

        case 'j':
            letra[0]=0x00;letra[1]=0x00;letra[2]=0x80;letra[3]=0xFA;letra[4]=0x00;letra[5]=0x00;letra[6]=0x00;
            break;

        case 'k':
            letra[0]=0x00;letra[1]=0x00;letra[2]=0xFE;letra[3]=0x20;letra[4]=0x50;letra[5]=0x88;letra[6]=0x00;
            break;

        case 'l':
            letra[0]=0x00;letra[1]=0x00;letra[2]=0x00;letra[3]=0xfe;letra[4]=0x00;letra[5]=0x00;letra[6]=0x00;
            break;

        case 'm':
            letra[0]=0x00;letra[1]=0xf0;letra[2]=0x10;letra[3]=0x20;letra[4]=0x10;letra[5]=0xf0;letra[6]=0x00;
            break;

        case 'n':
            letra[0]=0x00;letra[1]=0x00;letra[2]=0xF0;letra[3]=0x10;letra[4]=0xF0;letra[5]=0x00;letra[6]=0x00;
            break;

        case 'o':
            letra[0]=0x00;letra[1]=0x00;letra[2]=0xF8;letra[3]=0x88;letra[4]=0xF8;letra[5]=0x00;letra[6]=0x00;
            break;

        case 'p':
            letra[0]=0x00;letra[1]=0x00;letra[2]=0xfc;letra[3]=0x14;letra[4]=0x1c;letra[5]=0x00;letra[6]=0x00;
            break;

        case 'q':
            letra[0]=0x00;letra[1]=0x00;letra[2]=0x0E;letra[3]=0x0a;letra[4]=0xFe;letra[5]=0x00;letra[6]=0x00;
            break;

        case 'r':
            letra[0]=0x00;letra[1]=0x00;letra[2]=0xF8;letra[3]=0x10;letra[4]=0x08;letra[5]=0x00;letra[6]=0x00;
            break;

        case 't':
            letra[0]=0x00;letra[1]=0x00;letra[2]=0xfc;letra[3]=0xa0;letra[4]=0x80;letra[5]=0x00;letra[6]=0x00;
            break;

        case 's':
            letra[0]=0x00;letra[1]=0x00;letra[2]=0x98;letra[3]=0xa8;letra[4]=0xc8;letra[5]=0x00;letra[6]=0x00;
            break;

        case 'u':
            letra[0]=0x00;letra[1]=0x00;letra[2]=0xf0;letra[3]=0x80;letra[4]=0xf0;letra[5]=0x00;letra[6]=0x00;
            break;

        case 'v':
            letra[0]=0x00;letra[1]=0x20;letra[2]=0x40;letra[3]=0x80;letra[4]=0x40;letra[5]=0x20;letra[6]=0x00;
            break;

        case 'w':
            letra[0]=0x00;letra[1]=0x70;letra[2]=0x80;letra[3]=0x40;letra[4]=0x80;letra[5]=0x70;letra[6]=0x00;
            break;

        case 'x':
            letra[0]=0x00;letra[1]=0x00;letra[2]=0xa0;letra[3]=0x40;letra[4]=0xa0;letra[5]=0x00;letra[6]=0x00;
            break;

        case 'y':
            letra[0]=0x00;letra[1]=0x00;letra[2]=0x38;letra[3]=0xa0;letra[4]=0xf8;letra[5]=0x00;letra[6]=0x00;
            break;

        case 'z':
            letra[0]=0x00;letra[1]=0x00;letra[2]=0xc8;letra[3]=0xa8;letra[4]=0x98;letra[5]=0x00;letra[6]=0x00;
            break;


        //Mayúsuclas, espacio y default

        case 'A':
            letra[0]=0x00;letra[1]=0xFC;letra[2]=0x12;letra[3]=0x11;letra[4]=0x12;letra[5]=0xFC;letra[6]=0x00;
            break;

        case 'B':
            letra[0]=0x00;letra[1]=0x00;letra[2]=0xFF;letra[3]=0x99;letra[4]=0x66;letra[5]=0x00;letra[6]=0x00;
            break;

        case 'C':
            letra[0]=0x00;letra[1]=0x00;letra[2]=0xFF;letra[3]=0x81;letra[4]=0x81;letra[5]=0x00;letra[6]=0x00;
            break;

        case 'D':
            letra[0]=0x00;letra[1]=0x00;letra[2]=0xFF;letra[3]=0x81;letra[4]=0x7E;letra[5]=0x00;letra[6]=0x00;
            break;

        case 'E':
            letra[0]=0x00;letra[1]=0x00;letra[2]=0xFF;letra[3]=0xDB;letra[4]=0xDB;letra[5]=0x00;letra[6]=0x00;
            break;

        case 'F':
            letra[0]=0x00;letra[1]=0x00;letra[2]=0xFF;letra[3]=0x1B;letra[4]=0x1B;letra[5]=0x00;letra[6]=0x00;
            break;

        case 'G':
            letra[0]=0x00;letra[1]=0x00;letra[2]=0xFF;letra[3]=0x91;letra[4]=0xF3;letra[5]=0x00;letra[6]=0x00;
            break;

        case 'H':
            letra[0]=0x00;letra[1]=0x00;letra[2]=0xFF;letra[3]=0x18;letra[4]=0xFF;letra[5]=0x00;letra[6]=0x00;
            break;

        case 'I':
            letra[0]=0x00;letra[1]=0x00;letra[2]=0xC3;letra[3]=0xFF;letra[4]=0xC3;letra[5]=0x00;letra[6]=0x00;
            break;

        case 'J':
            letra[0]=0x00;letra[1]=0x00;letra[2]=0xE3;letra[3]=0x83;letra[4]=0xFF;letra[5]=0x00;letra[6]=0x00;
            break;

        case 'K':
            letra[0]=0x00;letra[1]=0x00;letra[2]=0xFF;letra[3]=0x3C;letra[4]=0x66;letra[5]=0xC3;letra[6]=0x00;
            break;

        case 'L':
            letra[0]=0x00;letra[1]=0x00;letra[2]=0xFF;letra[3]=0x80;letra[4]=0x80;letra[5]=0x00;letra[6]=0x00;
            break;

        case 'M':
            letra[0]=0x00;letra[1]=0xFF;letra[2]=0x07;letra[3]=0x18;letra[4]=0x07;letra[5]=0xFF;letra[6]=0x00;
            break;

        case 'N':
            letra[0]=0x00;letra[1]=0xFF;letra[2]=0x03;letra[3]=0x3C;letra[4]=0xC0;letra[5]=0xFF;letra[6]=0x00;
            break;

        case 'O':
            letra[0]=0x00;letra[1]=0x3C;letra[2]=0x43;letra[3]=0x81;letra[4]=0x42;letra[5]=0x3C;letra[6]=0x00;
            break;

        case 'P':
            letra[0]=0x00;letra[1]=0x00;letra[2]=0xFF;letra[3]=0x11;letra[4]=0x1F;letra[5]=0x00;letra[6]=0x00;
            break;

        case 'Q':
            letra[0]=0x00;letra[1]=0x00;letra[2]=0x7F;letra[3]=0x63;letra[4]=0x7F;letra[5]=0xC0;letra[6]=0x00;
            break;

        case 'R':
            letra[0]=0x00;letra[1]=0xFF;letra[2]=0x11;letra[3]=0x31;letra[4]=0x5F;letra[5]=0x80;letra[6]=0x00;
            break;

        case 'S':
            letra[0]=0x00;letra[1]=0x87;letra[2]=0x89;letra[3]=0x91;letra[4]=0xE1;letra[5]=0x00;letra[6]=0x00;
            break;

        case 'T':
            letra[0]=0x00;letra[1]=0x01;letra[2]=0xFF;letra[3]=0xFF;letra[4]=0x01;letra[5]=0x00;letra[6]=0x00;
            break;

        case 'U':
            letra[0]=0x00;letra[1]=0xFF;letra[2]=0x80;letra[3]=0x80;letra[4]=0x80;letra[5]=0xFF;letra[6]=0x00;
            break;

        case 'V':
            letra[0]=0x00;letra[1]=0x1F;letra[2]=0x60;letra[3]=0x80;letra[4]=0x60;letra[5]=0x1F;letra[6]=0x00;
            break;

        case 'W':
            letra[0]=0x00;letra[1]=0x7E;letra[2]=0x80;letra[3]=0x60;letra[4]=0x80;letra[5]=0x7E;letra[6]=0x00;
            break;

        case 'X':
            letra[0]=0x00;letra[1]=0xC3;letra[2]=0x3C;letra[3]=0x3C;letra[4]=0xC3;letra[5]=0x00;letra[6]=0x00;
            break;

        case 'Y':
            letra[0]=0x00;letra[1]=0x07;letra[2]=0x1F;letra[3]=0xF0;letra[4]=0x1F;letra[5]=0x07;letra[6]=0x00;
            break;

        case 'Z':
            letra[0]=0x00;letra[1]=0xC2;letra[2]=0xA2;letra[3]=0x92;letra[4]=0x8A;letra[5]=0x86;letra[6]=0x00;
            break;

        case ' ':
            letra[0]=0x00;letra[1]=0x00;letra[2]=0x00;letra[3]=0x00;letra[4]=0x00;letra[5]=0x00;letra[6]=0x00;
            break;
            
        case '?':
            letra[0]=0xC3;letra[1]=0x63;letra[2]=0x30;letra[3]=0x18;letra[4]=0x0C;letra[5]=0xC6;letra[6]=0xC3;
            break;
            
        case ':':
            letra[0]=0x00;letra[1]=0xEE;letra[2]=0x00;letra[3]=0x00;letra[4]=0x00;letra[5]=0x00;letra[6]=0x00;
            break;

        default:
            letra[0]=0x00;letra[1]=0x00;letra[2]=0x00;letra[3]=0x00;letra[4]=0x00;letra[5]=0x00;letra[6]=0x00;
            break;
    }
}

void LCD_send_Command(unsigned char* commands,unsigned int command_length )
{
    int x=0;
    for (x=0; x<command_length; x++)
    {
        wiringPiI2CWriteReg8 (fd_LCD, 0x00, commands[x]);
    }
}

void LCD_send_Data(unsigned char* data_, unsigned int data_length)
{
    int x=0;
    for (x=0; x<data_length; x++)
    {
        wiringPiI2CWriteReg8 (fd_LCD, 0x40, data_[x]);
    }
}

void LCD_init (void)
{
    unsigned char init_sequence[27] = {0xAE,0xD5,0x80,0xA8,63,0xD3,0x00,0x40,0x8D,
    0xD5,0x14,0x20,0x00,0xA1,0xC8,0xDA,0x12,0x81,0xCF,0xD9,0xF1, 0xDB,0x40,
    0xA4,0xA6,0x2E,0xAF};
    
    LCD_send_Command(init_sequence, 27);
}

void LCD_print()
{
    unsigned char start_screen[6]={0x22,0x00,0xFF,0x21,0x00,127};
    LCD_send_Command(start_screen,6);
    LCD_send_Data(LCD_screen_buffer,1024);
}

void LCD_clear()
{
    memset(&LCD_screen_buffer, 0, 1024);
    LCD_print();
}

void LCD_print_text()
{
    int y,z;
    char buffer_letra[8];

    for(y=0; y<18 ;y++)
    {
        LCD_parse(buffer_letra, LCD_line1[y]);
        for(z=0; z<7 ;z++)
        {
            LCD_screen_buffer[129+ (7*y) +z] = buffer_letra[0+z];
        }
    }
    LCD_print();
}

uint8_t HexChecksum(frame1 *DataCRC)
{
	uint8_t ChkSum = 1;
	ChkSum = DataCRC->SOF ^ DataCRC->SensorFrame[0];
	//printf("Checksum: %x\n", ChkSum);
	ChkSum = ChkSum ^ DataCRC->SensorFrame[1];
	//printf("Checksum: %x\n", ChkSum);
	ChkSum = ChkSum ^ DataCRC->SensorFrame[2];
	//printf("Checksum: %x\n", ChkSum);
	ChkSum = ChkSum ^ DataCRC->SensorFrame[3];
	//printf("Checksum: %x\n", ChkSum);
	ChkSum = ChkSum ^ DataCRC->Data;
	//printf("Checksum: %x\n", ChkSum);
	
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

int main (void)
{
    int32_t argv_copy_thread = (int) MAIN_PARAMETER_THREAD;

    //The thread identifier
    pthread_t tid[argv_copy_thread]; 
    
    if (pthread_mutex_init(&lock, NULL) != 0)
    {
        printf("\n mutex init failed\n");
        return 1;
    }
    
    pthread_create(&tid[0], NULL, threadInitLCD, MAIN_PARAMETER_THREAD);
    pthread_create(&tid[1], NULL, threadI2C, MAIN_PARAMETER_THREAD);
    pthread_create(&tid[2], NULL, threadServer, MAIN_PARAMETER_THREAD);
    pthread_create(&tid[3], NULL, threadHTML, MAIN_PARAMETER_THREAD);
    
    while(1)
    {
        
    }
    
    pthread_join(tid[0], NULL);
    pthread_join(tid[1], NULL);
    pthread_join(tid[2], NULL);
    pthread_join(tid[3], NULL);
    
    pthread_mutex_destroy(&lock);
    
    return 0;
}

void *threadInitLCD(void *param)
{
    pthread_mutex_lock(&lock);
    LCD_init();
    LCD_clear();
    sprintf(LCD_line1," niuuuuuum       ");
    LCD_print_text();
    pthread_mutex_unlock(&lock);
    pthread_exit(&tid[0]);
    
    return 0;
}

void *threadI2C(void *param)
{
        wiringPiSetup();
        fd_TI = wiringPiI2CSetup(SLAVE_ADDRESS_TI);
        fd_LCD = wiringPiI2CSetup(SLAVE_ADDRESS_LCD);
        
        
    while(1)
    {   
        pthread_mutex_lock(&lock);
        value = wiringPiI2CRead(fd_TI); 
        printf("\nTPS opening percentage: %i \r", value);
        sprintf(LCD_line1,"TPS OPENING :%d ?", value);
        LCD_print_text();
        newValue = (uint8_t) value;
        pthread_mutex_unlock(&lock);
    }

    return 0;
}

void *threadServer(void *param)
{
        //Data Frame Initialization
        uint8_t argv_copy = newValue;
        FTS = SensorFrameCreate(argv_copy);

        //Server Initialization
        int32_t server_fd; 
        int32_t new_socket; 
        struct sockaddr_in address;
        int32_t opt = 1;
        int32_t addrlen = sizeof(address);
        char buffer[1024] = {0};


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
        }
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port = htons( PORT );
           

        if (bind(server_fd, (struct sockaddr *)&address, 
                                     sizeof(address))<0)
        {
            perror("bind failed");
        }
        if (listen(server_fd, 3) < 0)
        {
            perror("listen");
        }
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, 
                           (socklen_t*)&addrlen))<0)
        {
            perror("accept");
        }
        while(1)
        {
            //pthread_mutex_lock(&lock);
            argv_copy = newValue;
            FTS = SensorFrameCreate(argv_copy);
            
            printf("Before server read\n");
            read( new_socket , buffer, 1024);
            printf("El dato es %i\n",argv_copy);
            printf("%s\n",buffer );
            printf("Before server send\n");
            send(new_socket , &FTS , sizeof(FTS) , 0 );
            printf("Hello message sent\n");

            //pthread_mutex_unlock(&lock);
        }
        
    return 0;
}

void *threadHTML(void *param)
{
    int32_t html_server_fd; 
    int32_t new_html_socket; 
    struct sockaddr_in address;
    int32_t option = 1;
    int32_t addresslen = sizeof(address);
    int32_t x = 3;
    
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
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT2 );
       
    
    if (bind(html_server_fd, (struct sockaddr *)&address, 
                                 sizeof(address))<0)
    {
        perror("bind failed");
    }
    if ((x = (listen(html_server_fd, 3)) < 0))
    {
        perror("listen");
    }
    if ((new_html_socket = accept(html_server_fd, (struct sockaddr *)&address, 
                       (socklen_t*)&addresslen))<0)
    {
        perror("accept");
    }
    while(1)
    {
        //pthread_mutex_lock(&lock);
        printf("Thread HTML\n");
        if(x == 0)
        {
            line4 = "TPS Opening Percentage: ";
            asprintf(&num, "%d", value);
            strcat(strcpy(sendBuffer, line4), num);
            strcat(sendBuffer, "\r");
            send(new_html_socket , sendBuffer , strlen(sendBuffer) , 0 );
            x = 1;
        }
        if ((x = (listen(html_server_fd, 1)) < 0))
        {
            perror("listen");
        }
        //pthread_mutex_unlock(&lock);
    }

    return 0;
}
