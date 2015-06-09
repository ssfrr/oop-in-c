#ifndef __REGISTERS_H
#define __REGISTERS_H

// fake register header file
#define TXDFULL 0x01
#define RXDFULL 0x02

// set it so the TXDFULL bit is always empty and the RXDFULL bit is always set
static unsigned char STATUS = RXDFULL;
static unsigned char RXD = 0x00;
static unsigned char TXD = 0x00;

#endif // __REGISTERS_H
