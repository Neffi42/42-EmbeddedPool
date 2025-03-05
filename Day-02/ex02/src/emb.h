#pragma once

#ifndef F_CPU
#define F_CPU 16000000
#endif

#ifndef UART_BAUDRATE
#define UART_BAUDRATE 115200
#endif

// UART Baud Rate Register in Asynchronous Double Speed | p.182
#define UBRRADS ((F_CPU / (8 * UART_BAUDRATE)) - 1)
