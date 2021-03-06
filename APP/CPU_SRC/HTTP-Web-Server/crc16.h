#ifndef CRC16_H_INCLUDED
#define CRC16_H_INCLUDED



#endif // CRC16_H_INCLUDED

/***** crc16.h *****/
//Tested
#define CRC16_DNP       0x3D65          // DNP, IEC 870, M-BUS, wM-BUS, ...
#define CRC16_CCITT     0x1021          // X.25, V.41, HDLC FCS, Bluetooth, ...

//Other polynoms not tested
#define CRC16_IBM       0x8005          // ModBus, USB, Bisync, CRC-16, CRC-16-ANSI, ...
#define CRC16_T10_DIF   0x8BB7          // SCSI DIF
#define CRC16_DECT      0x0589          // Cordeless Telephones
#define CRC16_ARINC     0xA02B          // ACARS Aplications


#define POLYNOM         CRC16_IBM // Define the used polynom from one of the aboves

// It calculates the new crc16 with the newByte. Variable crcValue is the actual or initial value (0).
unsigned int crc16(unsigned int crcValue, unsigned char newByte);
