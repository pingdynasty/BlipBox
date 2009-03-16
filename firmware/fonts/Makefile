BDFCONVERTER=../../tools/bdf-to-struct.pl

TARGETS=micro.c 6x10.c 6x9.c 5x8.c 5x7.c Misc-Fixed-Medium-R-Normal-6.c Misc-Fixed-Medium-R-Normal-7.c Misc-Fixed-Medium-R-Normal-8.c

all: $(TARGETS)

%.c: %.bdf
	$(BDFCONVERTER) $*.bdf > $@

clean:
	rm -f $(TARGETS)
