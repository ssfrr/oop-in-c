#include "UART.h"
#include "registers.h" // assume this has our register definitions

static UART_t *uart1 = NULL;
static UART_t *uart2 = NULL;

static unsigned char UART_rxBufFull(UART_t *uart);
static unsigned char UART_rxBufEmpty(UART_t *uart);
static unsigned char UART_txBufFull(UART_t *uart);
static unsigned char UART_txBufEmpty(UART_t *uart);
static void UART_isrCommon(UART_t *uart);

void UART_Init(UART_t *uart, UART_DevID_t devID) {
    uart->txReadPos = 0;
    uart->txWritePos = 0;
    uart->rxReadPos = 0;
    uart->rxWritePos = 0;
    switch(devID) {
        case UART1:
            // set the global uart pointer so we can access this from the ISR
            uart1 = uart;
            uart->txReg = &RXD1;
            uart->rxReg = &TXD1;
            uart->statusReg = &STATUS1;
            break;
        case UART2:
            // set the global uart pointer so we can access this from the ISR
            uart2 = uart;
            uart->txReg = &RXD2;
            uart->rxReg = &TXD2;
            uart->statusReg = &STATUS2;
            break;
        default:
            // just set to NULL so we'll segfault as soon as we try to access
            // them. In production an error code might be appropriate here.
            uart->txReg = NULL;
            uart->rxReg = NULL;
            uart->statusReg = NULL;
    }
}


// the interrupt enable/disable dance here is to prevent race conditions as we
// manipulate the FIFO buffer. Note that in production code you'd probably also
// want some sort of timeout.
void UART_writeByte(UART_t *uart, unsigned char outByte) {
    // block until we're ready to send another byte
    disableInterrupts();
    while(UART_txBufFull(uart)) {
        enableInterrupts();
        disableInterrupts();
    }
    if(UART_txBufEmpty(uart) && !(*(uart->statusReg) & TXDFULL)) {
        // no need to buffer the value, just send directly
        *(uart->txReg) = outByte;
    }
    else {
        uart->txBuf[uart->txWritePos] = outByte;
        uart->txWritePos = (uart->txWritePos + 1) % UART_BUFSIZE;
        if(uart->txWritePos == uart->txReadPos) {
            // buffer is full and we just overwrote the oldest value
            uart->txReadPos = (uart->txReadPos + 1) % UART_BUFSIZE;
        }
    }
    enableInterrupts();
}

// the interrupt enable/disable dance here is to prevent race conditions as we
// manipulate the FIFO buffer. Note that in production code you'd probably also
// want some sort of timeout.
unsigned char UART_readByte(UART_t *uart) {
    // block until there's data in the buffer
    unsigned char inByte;
    disableInterrupts();
    while(UART_rxBufEmpty(uart)) {
        enableInterrupts();
        disableInterrupts();
    }
    inByte = uart->rxBuf[uart->rxReadPos];
    uart->rxReadPos = (uart->rxReadPos + 1) % UART_BUFSIZE;
    enableInterrupts();
    return inByte;
}

static void UART_isrCommon(UART_t *uart) {
    if(*(uart->statusReg) & RXDFULL) {
        uart->rxBuf[uart->rxWritePos] = *(uart->rxReg);
        uart->rxWritePos = (uart->rxWritePos + 1) % UART_BUFSIZE;
        if(uart->rxWritePos == uart->rxReadPos) {
            // buffer is full and we just overwrote the oldest value
            uart->rxReadPos = (uart->rxReadPos + 1) % UART_BUFSIZE;
        }
    }
    if(!(*(uart->statusReg) & TXDFULL) && uart->txReadPos != uart->txWritePos) {
        // the transmit register is available and we have data in the transmit
        // buffer. Let's send it.
        *(uart->txReg) = uart->txBuf[uart->txReadPos];
        uart->txReadPos = (uart->txReadPos + 1) % UART_BUFSIZE;
    }
}

// these ISR functions get called whenever new data is available in the RXD
// register or when a transmission from the TX register is complete.
static void UART1_ISR(void) {
    if(uart1) {
        UART_isrCommon(uart1);
    }
}

static void UART2_ISR(void) {
    if(uart2) {
        UART_isrCommon(uart2);
    }
}

static unsigned char UART_rxBufFull(UART_t *uart) {
    return ((uart->rxWritePos + 1) % UART_BUFSIZE == uart->rxReadPos);
}

static unsigned char UART_txBufFull(UART_t *uart) {
    return ((uart->txWritePos + 1) % UART_BUFSIZE == uart->txReadPos);
}

static unsigned char UART_txBufEmpty(UART_t *uart) {
    return (uart->txWritePos == uart->txReadPos);
}

static unsigned char UART_rxBufEmpty(UART_t *uart) {
    return (uart->rxWritePos == uart->rxReadPos);
}

static unsigned char UART_txRegFull(UART_t *uart) {
    return *(uart->statusReg) & TXDFULL;
}

static void UART_writeTxReg(UART_t *uart) {
}
