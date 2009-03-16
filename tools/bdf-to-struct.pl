#!/usr/bin/perl -n
# script that converts a BDF font file to C source code
$FONTNAME = "typeface";
sub pad {
    my $count = shift;
    while($count-- > 0) { 
        print ", 0x00"; 
    }
}
if (/^FONTBOUNDINGBOX\s+(\d+)\s+(\d+)\s+(.+)\s+(.+)/) { 
    $WIDTH=$1;
    $HEIGHT=$2;
    $BASELINE=$4; # vertical offset of characters - negative or zero 
    printf("#define FONT_WIDTH %d\n", $WIDTH);
    printf("#define FONT_HEIGHT %d\n", $HEIGHT);
    printf("struct glyph { char c; uint8_t d[%d]; };\n", $HEIGHT); 
    printf("glyph %s[] = {\n", $FONTNAME); 
}
elsif (/^STARTCHAR\s+(.)$/) { 
    $INCHAR=1; 
    printf("    '%s', ", $1); 
}
elsif (/^BBX\s+(\d+)\s+(\d+)\s+(.+)\s+(.+)/ && $INCHAR) {
    $OFFSET=$HEIGHT-$2+$BASELINE-$4; # calculate how much top padding is needed
    print "{ ";
    if($OFFSET){
        print "0x00";
        pad($OFFSET-1);
    }
}
elsif (/^BITMAP/ && $INCHAR) { 
    $BITMAP=$OFFSET+1;
}
elsif (/^ENDCHAR/ && $INCHAR) { 
    pad($HEIGHT - $BITMAP + 1); 
    print " },\n"; 
    $BITMAP=0; 
    $INCHAR=0; 
}
elsif (/^ENDFONT/) { 
    print "    '\\0', { 0x00"; 
    pad($HEIGHT-1); print " }\n"; 
    print "};\n"; 
}
elsif (/^COPYRIGHT\s+(.+)$/) {
    printf("// %s\n", $1);
}
elsif ($BITMAP) { 
    s/(.+)\n$//; 
    printf($BITMAP++ == 1 ? "0x%s" : ", 0x%s", $1); 
}
