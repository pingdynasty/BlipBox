#!/usr/bin/perl -n
sub pad {
    my $count = shift;
    while($count-- >= 0) { print ", 0x00"; }
}
if (/^FONT\s+(\w+)/) { $FONTNAME = $1; }
elsif (/^FONT\s+/) { $FONTNAME = "typeface"; }
elsif (/^FONTBOUNDINGBOX\s+(\d+)\s+(\d+)/) { $HEIGHT = $2; printf("struct glyph {char c; uint8_t d[%d];};\n", $HEIGHT); printf("glyph %s[] = {\n", $FONTNAME); }
elsif (/^STARTCHAR\s+(.)$/) { $INCHAR=1; printf("    '%s', ", $1); }
elsif (/^BITMAP/ && $INCHAR) { $BITMAP=1; }
elsif (/^ENDCHAR/ && $INCHAR) { pad($HEIGHT - $BITMAP); print " },\n"; $BITMAP=0; $INCHAR=0; }
elsif (/^ENDFONT/) { print "    '\\0', { 0x00"; pad($HEIGHT-2); print " }\n"; print "};\n"; }
elsif ($BITMAP) { s/(.+)\n$//; printf($BITMAP++ == 1 ? "{ 0x%s" : ", 0x%s", $1); }
