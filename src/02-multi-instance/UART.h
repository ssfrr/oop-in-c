#ifndef __UART_H
#define __UART_H

#include <stddef.h>

#define UART_BUFSIZE 32

typedef enum {
    UART1,
    UART2
} UART_DevID_t;

typedef struct {
    unsigned char *txReg;
    unsigned char *rxReg;
    unsigned char *statusReg;
    unsigned char txBuf[UART_BUFSIZE];
    size_t txReadPos;
    size_t txWritePos;
    unsigned char rxBuf[UART_BUFSIZE];
    size_t rxReadPos;
    size_t rxWritePos;
} UART_t;

void UART_Init(UART_t *uart, UART_DevID_t devID);
void UART_writeByte(UART_t *uart, unsigned char outByte);
unsigned char UART_readByte(UART_t *uart);

#endif // __UART_H
