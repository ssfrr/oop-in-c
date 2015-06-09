#include <stdio.h>
#include <assert.h>
#include "UART.h"

int main(void) {
    const unsigned char outData[] = "Hello, World!\n";
    const unsigned char *idx = outData;
    UART_t uart;

    UART_Init(&uart, UART1);

    while(*idx) {
        UART_writeByte(&uart, *idx++);
    }

    printf("Msg Written\n");
    return 0;
}
