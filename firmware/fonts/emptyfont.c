#define FONT_WIDTH 3
#define FONT_HEIGHT 3
struct glyph { char c; uint8_t d[FONT_HEIGHT]; };
glyph typeface[] = {
  '\0', { 0x00, 0x00, 0x00 }
};
