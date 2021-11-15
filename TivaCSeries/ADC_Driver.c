#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_i2c.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_gpio.h"
#include "driverlib/i2c.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/interrupt.h"
#include "inc/hw_ints.h"
#include "driverlib/adc.h"

#define SLAVE_ADDRESS 0x55
uint32_t valorADC;
uint32_t porcentaje;


//initialize I2C module 0
//Slightly modified version of TI's example code
void InitI2C0(void)
{
    //enable I2C module 0
    SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C0);

    //reset module
    SysCtlPeripheralReset(SYSCTL_PERIPH_I2C0);

    //enable GPIO peripheral that contains I2C 0
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);

    // Configure the pin muxing for I2C0 functions on port B2 and B3.
    GPIOPinConfigure(GPIO_PB2_I2C0SCL);
    GPIOPinConfigure(GPIO_PB3_I2C0SDA);

    // Select the I2C function for these pins.
    GPIOPinTypeI2CSCL(GPIO_PORTB_BASE, GPIO_PIN_2);
    GPIOPinTypeI2C(GPIO_PORTB_BASE, GPIO_PIN_3);

    // Enable and initialize the I2C0 master module.  Use the system clock for
    // the I2C0 module.  The last parameter sets the I2C data transfer rate.
    // If false the data rate is set to 100kbps and if true the data rate will
    // be set to 400kbps.
    //I2CMasterInitExpClk(I2C0_BASE, SysCtlClockGet(), false);
    I2CSlaveEnable(I2C0_BASE);
    I2CSlaveInit(I2C0_BASE, SLAVE_ADDRESS);

    //clear I2C FIFOs
    HWREG(I2C0_BASE + I2C_O_FIFOCTL) = 80008000;
}

void I2C0SlaveIntHandler(void)
{
    //ADC
    ADCProcessorTrigger(ADC0_BASE,3);
    while(!ADCIntStatus(ADC0_BASE,3, false)){}
    ADCIntClear(ADC0_BASE,3);
    ADCSequenceDataGet(ADC0_BASE, 3,&valorADC);

    porcentaje = (valorADC*100)/4095;

    // Clear the I2C0 interrupt flag.
    I2CSlaveIntClear(I2C0_BASE);
    // Read the data from the slave.
    //Boton = I2CSlaveDataGet(I2C0_BASE);

    // Modifiy the data direction to true, so that seeing the address will indicate that the I2C Master is initiating a read from the slave.
    //I2CMasterSlaveAddrSet(I2C0_BASE, SLAVE_ADDRESS, true);
    I2CMasterSlaveAddrSet(I2C0_BASE, SLAVE_ADDRESS, true);

    // Send data to the master
    I2CSlaveDataPut(I2C0_BASE, porcentaje);
    while(!(I2CSlaveStatus(I2C0_BASE) & I2C_SLAVE_ACT_TREQ))
    {
    }
    //SysCtlDelay(SysCtlClockGet()/30);

}

void main(void)
{
    //uint32_t lectura;
    //uint32_t ReadI2C;

    SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);

    //Configure ADC
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOE));
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_ADC0));

    //ADC
    GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_3);
    ADCSequenceConfigure(ADC0_BASE, 3, ADC_TRIGGER_PROCESSOR, 0);
    ADCSequenceStepConfigure(ADC0_BASE,3, 0, ADC_CTL_CH0 | ADC_CTL_IE | ADC_CTL_END);
    ADCSequenceEnable(ADC0_BASE, 3);
    ADCIntClear(ADC0_BASE, 3);


    InitI2C0();

    IntEnable(INT_I2C0);
    I2CSlaveIntEnableEx(I2C0_BASE, I2C_SLAVE_INT_DATA);
    I2CIntRegister(I2C0_BASE, I2C0SlaveIntHandler);
    IntMasterEnable();

    while(1)
    {
    }
}
