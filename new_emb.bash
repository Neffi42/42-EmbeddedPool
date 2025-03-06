#!/bin/bash

set -e

if [ "$#" -lt 1 ] || [ "$#" -gt 2 ]; then
	echo "Usage: $0 <target directory> [program name]"
	exit 1
fi

TARGET_DIR=$1
PROGRAM_NAME=${2:-main}

mkdir -pv "$TARGET_DIR/src"
touch "$TARGET_DIR/src/$PROGRAM_NAME.c"

cat << EOF > "$TARGET_DIR/Makefile"
include ../../config.mk

NAME		:= $PROGRAM_NAME
SRC_DIR		:= ./src
BUILD_DIR	:= ./build

.PHONY: all bin hex flash screen

all: flash

\$(BUILD_DIR):
	mkdir -p \$(BUILD_DIR)

bin: \$(BUILD_DIR)
	avr-gcc -mmcu=atmega328p -Wall -Werror -Os -D F_CPU=\$(F_CPU) -D UART_BAUDRATE=\$(UART_BAUDRATE) -o \$(BUILD_DIR)/\$(NAME).bin \$(SRC_DIR)/\$(NAME).c

hex: bin
	avr-objcopy -j .text -j .data -O ihex \$(BUILD_DIR)/\$(NAME).bin \$(BUILD_DIR)/\$(NAME).hex

flash: hex
	avrdude -c arduino -p m328p -P \$(PORT) -b \$(UART_BAUDRATE) -U flash:w:\$(BUILD_DIR)/\$(NAME).hex

clean:
	rm -rf \$(BUILD_DIR)

screen:
	screen \$(PORT) \$(UART_BAUDRATE)
EOF

cat << EOF > "$TARGET_DIR/src/emb.h"
#pragma once

#ifndef F_CPU
#define F_CPU 16000000
#endif

#ifndef UART_BAUDRATE
#define UART_BAUDRATE 115200
#endif

// UART Baud Rate Register in Asynchronous Double Speed | p.182
#define UBRRADS ((F_CPU / (8 * UART_BAUDRATE)) - 1)
EOF
