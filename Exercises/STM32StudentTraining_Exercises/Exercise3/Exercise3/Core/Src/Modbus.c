/*
 * Modbus_handler.c
 *
 */

#include "main.h"
#include <string.h>

unsigned short uchCRC;
unsigned short uchCRC_input;

unsigned char data[15];

unsigned short HR[8];
int hrIndex;

unsigned char CRCch;
unsigned char CRCcl;
int TxPacketReady = 0, TxPacketSize;
extern UART_HandleTypeDef huart2;

unsigned short CRC16(unsigned char*, unsigned short);

void Modbus_handler(uint8_t *buffer) {
	int	i;

	uchCRC_input = CRC16(buffer, (unsigned short)6);	// calculate Rx CRC

	CRCcl = (unsigned char) (uchCRC_input & 0xFF);
	CRCch = (unsigned char) (uchCRC_input >> 8);

	if (buffer[6] != CRCcl || buffer[7] != CRCch) {		// bad CRC? just return
		return;
	}

	if(buffer[1] == 0x06)	// handle only FC 06
	{
		hrIndex = (((unsigned short)buffer[2]) << 8) | (unsigned short)buffer[3];	// the register address

		HR[hrIndex] = (((unsigned short)buffer[4]) << 8) | (unsigned short)buffer[5];

		for(i = 0; i < 8; i++)
		{
			data[i] = buffer[i];
		}
		TxPacketSize = i;
		TxPacketReady = 1;
														// TO DO #1

														// TO DO #2
														// TO DO #3

														// TO DO #4

														// TO DO #5
	}

}
