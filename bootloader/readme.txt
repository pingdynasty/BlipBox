
Plug in avrusb500 programmer with chip in zif socket.

Build and upload bootloader to atmega328 with:
make atmega328_isp

Build and upload bootloader to atmega168 with:
make diecimila_isp

Works with the following programmer settings (set in Makefile):
ISPTOOL	   = stk500v2
ISPPORT	   = /dev/tty.usbserial-A9003Tc5
ISPSPEED   = -b 115200
