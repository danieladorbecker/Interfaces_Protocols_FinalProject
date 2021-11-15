#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <stdint.h>
#include <string.h>

#define MAIN_PARAMETER_1 1

int SensFrmCount = 0;

typedef struct
{
	uint8_t SOF;
	char SensorFrame[4];
	uint8_t Data;
	uint8_t Checksum;
}frame1;

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
	
	printf("Data Frame \nSOF: %x \nSensor Frame: %x, %x, %x, %x \nData: %x\n", DataCRC->SOF, DataCRC->SensorFrame[0], DataCRC->SensorFrame[1], DataCRC->SensorFrame[2], DataCRC->SensorFrame[3], DataCRC->Data);
	
	return ChkSum;
}

void SensorFrameCreate(uint8_t frameData)
{
	frame1 FrameToSend;

	//Defining frame1 values
    FrameToSend.SOF = 0x15;	//0x15
   	FrameToSend.SensorFrame[0] = 0x54;	//TIVA
   	FrameToSend.SensorFrame[1] = 0x49;	//TIVA
   	FrameToSend.SensorFrame[2] = 0x56;	//TIVA
   	FrameToSend.SensorFrame[3] = 0x41;	//TIVA
   	FrameToSend.Data = frameData;	//User provided data
   	FrameToSend.Checksum = HexChecksum(&FrameToSend);
   	printf("Checksum: %x\n", FrameToSend.Checksum);
}

int32_t main(int32_t argc, char *argv[])
{
	uint8_t argv_copy = atoi(argv[MAIN_PARAMETER_1]);
	SensorFrameCreate(argv_copy);
}
