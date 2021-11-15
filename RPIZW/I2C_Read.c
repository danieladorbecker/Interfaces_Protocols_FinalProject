#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <stdio.h>

#define SLAVE_ADDRESS 0x10

int main (void)
{
    printf("I2C Init \n");
    int fd;
    int value;

    wiringPiSetup();
    fd = wiringPiI2CSetup(SLAVE_ADDRESS);

    while (1)
    {
        value = wiringPiI2CRead(fd); 
        printf("TPS opening percentage: %i \n", value);
        
        delay(10);
    }
    return 0;
}
