fonts:
start X font server: xfs
list available fonts: fslsfonts -server localhost:font-service
get font as BDF: fstobdf -server localhost:font-service -fn micro

installed unifoundry perl scripts
convert BDF font to uni hex format: bdfimplode 5x8.bdf > 5x8.hex
viewing with hexdraw does not seem to work: hexdraw < 5x8.hex 
probably because bdfimplode does not properly convert bdf file

generate C struct from BDF font file with e.g.:
./tools/bdf-to-struct.pl firmware/fonts/6x9.bdf > firmware/fonts/6x9.c
for nm in firmware/fonts/*.bdf; do ./tools/bdf-to-struct.pl $nm > `basename $nm .bdf`.c; done

note: font width and height may be off by one.

Makefile:
Implicit rules available for .hex (unifont), .c (c struct) and .txt (hexdraw text output)
Generate hexdraw ascii art output with e.g. make 6x9.txt
Generate all .hex and .c font files from bdf sources with make all
