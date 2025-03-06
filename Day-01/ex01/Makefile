include ../../config.mk

NAME		:= main
SRC_DIR		:= ./src
BUILD_DIR	:= ./build

.PHONY: all bin hex flash

all: flash

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

bin: $(BUILD_DIR)
	avr-gcc -mmcu=atmega328p -Wall -Werror -Os -D F_CPU=$(F_CPU) -o $(BUILD_DIR)/$(NAME).bin $(SRC_DIR)/$(NAME).c

hex: bin
	avr-objcopy -j .text -j .data -O ihex $(BUILD_DIR)/$(NAME).bin $(BUILD_DIR)/$(NAME).hex

flash: hex
	avrdude -c arduino -p m328p -P $(PORT) -b $(UART_BAUDRATE) -U flash:w:$(BUILD_DIR)/$(NAME).hex

clean:
	rm -rf $(BUILD_DIR)
