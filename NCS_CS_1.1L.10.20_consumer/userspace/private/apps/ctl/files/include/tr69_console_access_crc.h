#ifndef __TR69_CONSOLE_ACCESS_CRC_H__
#define __TR69_CONSOLE_ACCESS_CRC__

unsigned char fcsCalculate( unsigned char *Cell, UINT32 Length, unsigned char fcs);
void Create_CRC_Byte(UINT32 *num);
int Verify_CRC_Byte(UINT32* num);

#endif

