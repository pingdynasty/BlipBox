C_CONVERTER=../../tools/bdf-to-struct.pl
HEX_CONVERTER=../../tools/bdf-to-unifont.pl
HEXDRAW=hexdraw

TARGETS=micro.c 6x10.c 6x9.c 5x8.c 5x7.c Misc-Fixed-Medium-R-Normal-6.c Misc-Fixed-Medium-R-Normal-7.c Misc-Fixed-Medium-R-Normal-8.c

all: $(TARGETS)

%.c: %.bdf
	$(C_CONVERTER) $*.bdf > $@

%.hex: %.bdf
	$(HEX_CONVERTER) $*.bdf > $@

draw: $(TARGET).hex
	$(HEXDRAW) $(TARGET).hex

clean:
	rm -f $(TARGETS) *.hex
