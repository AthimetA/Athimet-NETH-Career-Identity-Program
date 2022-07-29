/*
 * Modbus_handler.c
 *
 */

#include "main.h"
#include <string.h>

unsigned short uchCRC;
unsigned short uchCRC_input;

unsigned char data[15];

unsigned short HR[5];
int hrIndex;

unsigned char CRCch;
unsigned char CRCcl;
int TxPacketReady = 0, TxPacketSize;
extern UART_HandleTypeDef huart2;

unsigned short CRC16(unsigned char*, unsigned short);

void Modbus_handler(uint8_t *buffer) {
	int	i, TotalBytes;

	uchCRC_input = CRC16(buffer, (unsigned short)6);	// calculate Rx CRC

	CRCcl = (unsigned char) (uchCRC_input & 0xFF);
	CRCch = (unsigned char) (uchCRC_input >> 8);

	if (buffer[6] != CRCcl || buffer[7] != CRCch) {		// bad CRC? just return
		return;
	}

	// handle only FC 03
	if(buffer[1] == 0x03)
	{
		data[0] = 0x01;		// slave address
		data[1] = 0x03;		// FC 03

		data[2] = (unsigned char)((int)buffer[5] * 2);	// no. of data in bytes

		hrIndex = (((unsigned short)buffer[2]) << 8) | (unsigned short)buffer[3];	// start of data

		TotalBytes = (((int)buffer[4] * 256) + (int)buffer[5]) * 2;	// calculate total bytes to send back

		for(i = 3; i < TotalBytes + 2; i = i + 2)		// fill the requested data
		{
			if(hrIndex < 5)
			{
				data[i] = (unsigned char)(HR[hrIndex] >> 8);
				data[i+1] = (unsigned char)(HR[hrIndex++] & 0xFF);
			}
			else data[i] = 0;	// zero unprovided data
		}

		TxPacketSize = i + 2;	// Tx packet size must include 2 bytes for CRC
		uchCRC = CRC16(data, (unsigned short)i);	// calculate Tx CRC and fill them in Tx packet

		data[i] = (unsigned char)(uchCRC & 0xFF);
		data[i+1] = (unsigned char)(uchCRC >> 8);
	}
	else if(buffer[1] == 0x06)
	{
		// do the code
	}

	TxPacketReady = 1;	// signal main loop to handle packet transmission
}
