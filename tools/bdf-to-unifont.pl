#!/usr/bin/perl -n
# script that converts a BDF font file to GNU Unifont format
sub pad {
    my $count = shift;
    while($count-- > 0) { 
        print "00"; 
    }
}
if (/^FONTBOUNDINGBOX\s+(\d+)\s+(\d+)\s+(.+)\s+(.+)/) { 
    $HEIGHT=16; # always produce 16 pixel high glyphs for unifont format
    $BASELINE=$4; # vertical offset of characters - generally negative or zero 
}
elsif (/^ENCODING\s+(\d+)/) { 
    printf ("%04X:", $1); 
    $INCHAR=1; 
}
elsif (/^BBX\s+(\d+)\s+(\d+)\s+(.+)\s+(.+)/ && $INCHAR) {
    $OFFSET=$HEIGHT-$2+$BASELINE-$4; # calculate how much top padding is needed
    pad($OFFSET);
}
elsif (/^BITMAP/ && $INCHAR) { 
    $BITMAP=$OFFSET+1;
}
elsif (/^ENDCHAR/ && $INCHAR) { 
    pad($HEIGHT - $BITMAP + 1); 
    print "\n";
    $BITMAP=0; 
    $INCHAR=0; 
}
elsif ($BITMAP) { 
    y/a-f/A-F/; s/\n$//; 
    print;
    $BITMAP++;
}
