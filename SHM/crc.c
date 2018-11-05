#include <stdio.h>

#define CRC_POLYNOMIAL		(0xEDB88320)
#define CRC_TABLE_SIZE		(256)

unsigned int crcTable[CRC_TABLE_SIZE];

void crc_init(void)
{
	int i, j;
	unsigned int crc;

	for (i = 0; i < CRC_TABLE_SIZE; i++)
	{
		crc = i;
		for (j = 8; j > 0; j--)
		{
			if ((crc & 1) == 1)
				crc = (crc >> 1) ^ CRC_POLYNOMIAL;
			else
				crc = crc >> 1;
		}
		
		crcTable[i] = crc;
	}
}

unsigned int crc_compute(char *buf, int nbytes)
{

	unsigned int crc;
	unsigned int t1, t2;

	crc = 0xFFFFFFFF;
	while (nbytes-- != 0)
	{
		t1 = crc >> 8;
		t2 = crcTable[(crc ^ *buf++) & 0xff];
		crc = t1 ^ t2;
	}
	
	return (crc);
}
