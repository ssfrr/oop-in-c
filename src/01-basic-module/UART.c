#include "UART.h"
#include "registers.h" // assume this has our register definitions

void UART_writeByte(unsigned char outByte) {
    // block until we're ready to send another byte
    while(STATUS & TXDFULL) {
    }
    TXD = outByte;
}

unsigned char UART_readByte(void) {
    // block until there's new data received
    while(!(STATUS & RXDFULL)) {
    }
    return RXD;
}
