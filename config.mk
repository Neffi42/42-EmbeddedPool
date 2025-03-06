F_CPU			:= 16000000
UART_BAUDRATE	:= 115200

ifeq ($(shell uname -s), Darwin)
    PORT := /dev/tty.usbserial-10
else
    PORT := /dev/ttyS0
endif
