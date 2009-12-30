#!/usr/bin/perl -n
# script that converts a BDF font file to C source code
sub pad {
    my $count = shift;
    while($count-- > 0) { 
        print ", 0x00"; 
    }
}
if (/^FONT\s+(.*)$/) { 
    $FONTNAME = $1;
}
elsif (/^FONTBOUNDINGBOX\s+(\d+)\s+(\d+)\s+(.+)\s+(.+)/) { 
    $WIDTH= $1 ;
    $HEIGHT= $2 ;
    $BASELINE= $4; # vertical offset of characters - negative or zero 
    printf("/* Font name: %s */\n", $FONTNAME); 
    printf("#define FONT_WIDTH %d\n", $WIDTH);
    printf("#define FONT_HEIGHT %d\n", $HEIGHT);
    print "PROGMEM prog_uchar typeface[]  = {\n";
#     print "struct glyph { char c; uint8_t d[FONT_HEIGHT]; };\n"; 
#     print "glyph typeface[] = {\n"; 
}
# elsif (/^STARTCHAR\s+(.)$/) {  # only matches single char symbols: 0-9, a-z, A-Z
elsif (/^STARTCHAR\s+(.*)$/) { 
    printf("\t/* %s */\n", $1);
}
elsif (/^ENCODING\s+([0-9]+)$/) { 
    if($1 > 32 && $1 < 127){
        $INCHAR=1; 
        printf("\t%s, ", $1); 
    }
}
elsif (/^BBX\s+(\d+)\s+(\d+)\s+(.+)\s+(.+)/ && $INCHAR) {
    $OFFSET=$HEIGHT-$2+$BASELINE-$4; # calculate how much top padding is needed
    if($OFFSET < 0){
        printf("/* bdf-to-struct.pl ERROR! Negative offset: %s */", $OFFSET);
    }
#     print "{ ";
    if($OFFSET > 0){
        print "0x00";
        pad($OFFSET-1);
    }
}
elsif (/^BITMAP/ && $INCHAR) { 
    $BITMAP=$OFFSET+1;
}
elsif (/^ENDCHAR/ && $INCHAR) { 
    pad($HEIGHT - $BITMAP + 1); 
    print ",\n"; 
#     print " },\n"; 
    $BITMAP=0; 
    $INCHAR=0; 
}
elsif (/^ENDFONT/) { 
    print "    '\\0', 0x00"; 
    pad($HEIGHT-1); print "\n"; 
#     print "    '\\0', { 0x00"; 
#     pad($HEIGHT-1); print " }\n"; 
    print "};\n"; 
}
elsif (/^COPYRIGHT\s+(.+)$/) {
    printf("/* Copyright: %s */\n", $1);
}
elsif ($BITMAP) { 
    s/(.+)\n$//; 
    printf($BITMAP++ == 1 ? "0x%s" : ", 0x%s", $1); 
}
