author: Spencer Russell
comments: true
title: A Gentle Introduction to Object-Oriented C
Status: draft


Let's say we are creating a simple module for UART communication. The exact
semantics of the hardware aren't important, but let's assume we have the
registers:
* `TXD` will send a byte out when you write to it
* `RXD` which contains the last byte received
* `STATUS` has bit flags for `TXDFULL` (0x01)
* `RXDFULL` (0x02) which indicated whether there's new (unsent or unread) data
  in the respective registers.


Let's start with a first pass

### UART.h

```c
void writeByte(unsigned char outByte);
unsigned char readByte(void);
```

### UART.c

```c
#include "UART.h"
#include "registers.h" // assume this has our register definitions

#define TXDFULL 0x01
#define RXDFULL 0x02

void writeByte(unsigned char outByte) {
    // block until we're ready to send another byte
    while(STATUS & TXDFULL) {
    }
    TXD = outByte;
}

unsigned char readByte(void) {
    // block until there's new data received
    while(!(STATUS & RXDFULL) {
    }
    return RXD;
}
```

### main.c

```c
#include "UART.h"

int main(void) {
    const unsigned char outData[] = "Hello, World!\n";
    const unsigned char *idx = outData;

    while(idx) {
          writeByte(*idx++);
    }
    return 0;
}
```
