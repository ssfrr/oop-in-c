#ifndef __REGISTERS_H
#define __REGISTERS_H

// fake register header file
#define TXDFULL 0x01
#define RXDFULL 0x02

// set it so the TXDFULL bit is always empty and the RXDFULL bit is always set
static unsigned char STATUS1 = RXDFULL;
static unsigned char RXD1 = 0x00;
static unsigned char TXD1 = 0x00;
static unsigned char STATUS2 = RXDFULL;
static unsigned char RXD2 = 0x00;
static unsigned char TXD2 = 0x00;

static void disableInterrupts(void) {}
static void enableInterrupts(void) {}

#endif // __REGISTERS_H
