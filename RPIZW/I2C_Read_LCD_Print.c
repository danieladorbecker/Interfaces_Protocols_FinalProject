#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define SLAVE_ADDRESS_TI 0x10
#define SLAVE_ADDRESS_LCD 0x3C

int fd_LCD;
int fd_TI;
unsigned char LCD_screen_buffer[1024];
char LCD_line1[18];
int value;

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

void threadI2C(void)
{
    printf("I2C Init \n");
    
    wiringPiSetup();
    fd_TI = wiringPiI2CSetup(SLAVE_ADDRESS_TI);
    fd_LCD = wiringPiI2CSetup(SLAVE_ADDRESS_LCD);
    
    LCD_init();
    LCD_clear();
    sprintf(LCD_line1," niuuuuuum       ");
    LCD_print_text();
    delay(2000);
    
    while (1)
    {
        value = wiringPiI2CRead(fd_TI); 
        printf("TPS opening percentage: %i \n", value);
        sprintf(LCD_line1,"TPS OPENING : %d ?", value);
        LCD_print_text();
        
        delay(10);
    }
}

int main (void)
{
    threadI2C();
    
    return 0;
}
