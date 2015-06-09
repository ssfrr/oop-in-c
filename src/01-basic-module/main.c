#include <stdio.h>
#include <assert.h>
#include "UART.h"

int main(void) {
    const unsigned char outData[] = "Hello, World!\n";
    const unsigned char *idx = outData;

    while(*idx) {
        UART_writeByte(*idx++);
    }

    printf("Msg Written\n");
    return 0;
}
