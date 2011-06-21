#!/bin/bash
PORT=/dev/tty.usbserial-A9003Tc5
PROG=avrusb500
MCU=m168
HEX=/Users/mars/arduino/arduino-0016/hardware/bootloaders/atmega/ATmegaBOOT_168_diecimila.hex
check="avrdude -c $PROG -p $MCU -v -P $PORT"
fuses="avrdude -c $PROG -p $MCU -U lfuse:w:0xff:m -U hfuse:w:0xdd:m -U efuse:w:0x00:m -P $PORT"
burn="avrdude -c $PROG -p $MCU -U flash:w:$HEX  -P $PORT"
$check && $fuses && $burn