#include "glyphs.h"
#include "fb.h"

struct glyph {
   uint8_t w; // actual glyph width in pels
   uint8_t mask[5*5+1]; // pixels we actually fill with fg color
};

// using ascii chars here, for maintainability
struct glyph const alnum[10+26+1] = {
   { 5, "11111" // 0
        "10001"
        "10001"
        "10001"
        "11111" },
   { 2, "11000" // 1
        "01000"
        "01000"
        "01000"
        "01000" },
   { 5, "11111" // 2
        "00001"
        "11111"
        "10000"
        "11111" },
   { 5, "11111" // 3
        "00001"
        "01111"
        "00001"
        "11111" },
   { 5, "10001" // 4
        "10001"
        "11111"
        "00001"
        "00001" },
   { 5, "11111" // 5
        "10000"
        "11111"
        "00001"
        "11111" },
   { 5, "11111" // 6
        "10000"
        "11111"
        "10001"
        "11111" },
   { 5, "11111" // 7
        "00001"
        "00001"
        "00001"
        "00001" },
   { 5, "11111" // 8
        "10001"
        "11111"
        "10001"
        "11111" },
   { 5, "11111" // 9
        "10001"
        "11111"
        "00001"
        "11111" },

   { 5, "11111" // A
        "10001"
        "11111"
        "10001"
        "10001" },
   { 5, "11110" // B
        "10001"
        "11111"
        "10001"
        "11110" },
   { 4, "11110" // C
        "10000"
        "10000"
        "10000"
        "11110" },
   { 5, "11110" // D
        "10001"
        "10001"
        "10001"
        "11110" },
   { 5, "11111" // E
        "10000"
        "11110"
        "10000"
        "11111" },
   { 5, "11111" // F
        "10000"
        "11110"
        "10000"
        "10000" },
   { 5, "11111" // G
        "10000"
        "10011"
        "10001"
        "11111" },
   { 5, "10001" // H
        "10001"
        "11111"
        "10001"
        "10001" },
   { 1, "10000" // I
        "10000"
        "10000"
        "10000"
        "10000" },
   { 4, "11110" // J
        "00010"
        "00010"
        "10010"
        "01100" },
   { 4, "10010" // K
        "10100"
        "11000"
        "10100"
        "10010" },
   { 4, "10000" // L
        "10000"
        "10000"
        "10000"
        "11110" },
   { 5, "11111" // M
        "10101"
        "10101"
        "10101"
        "10101" },
   { 5, "11111" // N
        "10001"
        "10001"
        "10001"
        "10001" },
   { 5, "11111" // O
        "10001"
        "10001"
        "10001"
        "11111" },
   { 5, "11111" // P
        "10001"
        "11111"
        "10000"
        "10000" },
   { 5, "11111" // Q
        "10001"
        "10001"
        "10011"
        "11111" },
   { 5, "11110" // R
        "10010"
        "11110"
        "10100"
        "10010" },
   { 5, "11111" // S
        "10000"
        "11111"
        "00001"
        "11111" },
   { 5, "11111" // T
        "00100"
        "00100"
        "00100"
        "00100" },
   { 5, "10001" // U
        "10001"
        "10001"
        "10001"
        "11111" },
   { 5, "10001" // V
        "10001"
        "01010"
        "01010"
        "00100" },
   { 5, "10001" // W
        "10001"
        "10101"
        "11011"
        "10001" },
   { 5, "10001" // X
        "01010"
        "00100"
        "01010"
        "10001" },
   { 5, "10001" // Y
        "10001"
        "01110"
        "00100"
        "00100" },
   { 5, "11111" // Z
        "00010"
        "00100"
        "01000"
        "11111" },

   { 14,"00000" // space
        "00000"
        "00000"
        "00000"
        "00000" }
};

#ifdef GLYPH_SIZE_1
static void drawGlyph1(GPoint p, int8_t g, uint8_t c) {
   uint8_t *fb = fbSet(NULL);
   for (int x = 0; x < 5; x++)
      for (int y = 0; y < 5; y++)
         if (alnum[g].mask[x+y*5] == '1')
            fb[p.x+x + (p.y+y) * FBW] = c;
   fbSet(fb);
}
#endif

static void drawGlyph2(GPoint p, int8_t g, uint8_t c) {
   uint8_t *fb = fbSet(NULL);
   for (int x = 0; x < 5; x++)
      for (int y = 0; y < 5; y++)
         if (alnum[g].mask[x+y*5] == '1') {
            int bx = p.x+x*2;
            int by = p.y+y*2;
            fb[bx   + (by)   * FBW] = c;
            fb[bx+1 + (by)   * FBW] = c;
            fb[bx   + (by+1) * FBW] = c;
            fb[bx+1 + (by+1) * FBW] = c;
         }
   fbSet(fb);
}

#ifdef GLYPH_SIZE_3
static void drawGlyph3(GPoint p, int8_t g, uint8_t c) {
   uint8_t *fb = fbSet(NULL);
   for (int x = 0; x < 5; x++)
      for (int y = 0; y < 5; y++)
         if (alnum[g].mask[x+y*5] == '1') {
            int bx = p.x+x*3;
            int by = p.y+y*3;
            fb[bx   + (by)   * FBW] = c;
            fb[bx+1 + (by)   * FBW] = c;
            fb[bx+2 + (by)   * FBW] = c;
            fb[bx   + (by+1) * FBW] = c;
            fb[bx+1 + (by+1) * FBW] = c;
            fb[bx+2 + (by+1) * FBW] = c;
            fb[bx   + (by+2) * FBW] = c;
            fb[bx+1 + (by+2) * FBW] = c;
            fb[bx+2 + (by+2) * FBW] = c;
         }
   fbSet(fb);
}
#endif

#ifdef GLYPH_SIZE_4
static void drawGlyph4(GPoint p, int8_t g, uint8_t c) {
   uint8_t *fb = fbSet(NULL);
   for (int x = 0; x < 5; x++)
      for (int y = 0; y < 5; y++)
         if (alnum[g].mask[x+y*5] == '1') {
            int bx = p.x+x*4;
            int by = p.y+y*4;
            fb[bx   + (by)   * FBW] = c;
            fb[bx+1 + (by)   * FBW] = c;
            fb[bx+2 + (by)   * FBW] = c;
            fb[bx+3 + (by)   * FBW] = c;
            fb[bx   + (by+1) * FBW] = c;
            fb[bx+1 + (by+1) * FBW] = c;
            fb[bx+2 + (by+1) * FBW] = c;
            fb[bx+3 + (by+1) * FBW] = c;
            fb[bx   + (by+2) * FBW] = c;
            fb[bx+1 + (by+2) * FBW] = c;
            fb[bx+2 + (by+2) * FBW] = c;
            fb[bx+3 + (by+2) * FBW] = c;
            fb[bx   + (by+3) * FBW] = c;
            fb[bx+1 + (by+3) * FBW] = c;
            fb[bx+2 + (by+3) * FBW] = c;
            fb[bx+3 + (by+3) * FBW] = c;
         }
   fbSet(fb);
}
#endif

static void drawGlyph5(GPoint p, int8_t g, uint8_t c) {
   uint8_t *fb = fbSet(NULL);
   for (int x = 0; x < 5; x++)
      for (int y = 0; y < 5; y++)
         if (alnum[g].mask[x+y*5] == '1') {
            int bx = p.x+x*5;
            int by = p.y+y*5;
            fb[bx   + (by)   * FBW] = c;
            fb[bx+1 + (by)   * FBW] = c;
            fb[bx+2 + (by)   * FBW] = c;
            fb[bx+3 + (by)   * FBW] = c;
            fb[bx+4 + (by)   * FBW] = c;
            fb[bx   + (by+1) * FBW] = c;
            fb[bx+1 + (by+1) * FBW] = c;
            fb[bx+2 + (by+1) * FBW] = c;
            fb[bx+3 + (by+1) * FBW] = c;
            fb[bx+4 + (by+1) * FBW] = c;
            fb[bx   + (by+2) * FBW] = c;
            fb[bx+1 + (by+2) * FBW] = c;
            fb[bx+2 + (by+2) * FBW] = c;
            fb[bx+3 + (by+2) * FBW] = c;
            fb[bx+4 + (by+2) * FBW] = c;
            fb[bx   + (by+3) * FBW] = c;
            fb[bx+1 + (by+3) * FBW] = c;
            fb[bx+2 + (by+3) * FBW] = c;
            fb[bx+3 + (by+3) * FBW] = c;
            fb[bx+4 + (by+3) * FBW] = c;
            fb[bx   + (by+4) * FBW] = c;
            fb[bx+1 + (by+4) * FBW] = c;
            fb[bx+2 + (by+4) * FBW] = c;
            fb[bx+3 + (by+4) * FBW] = c;
            fb[bx+4 + (by+4) * FBW] = c;
         }
   fbSet(fb);
}

#ifdef GLYPH_SIZE_6
static void drawGlyph6(GPoint p, int8_t g, uint8_t c) {
   uint8_t *fb = fbSet(NULL);
   for (int x = 0; x < 5; x++)
      for (int y = 0; y < 5; y++)
         if (alnum[g].mask[x+y*5] == '1') {
            int bx = p.x+x*5;
            int by = p.y+y*5;
            fb[bx   + (by)   * FBW] = c;
            fb[bx+1 + (by)   * FBW] = c;
            fb[bx+2 + (by)   * FBW] = c;
            fb[bx+3 + (by)   * FBW] = c;
            fb[bx+4 + (by)   * FBW] = c;
            fb[bx+5 + (by)   * FBW] = c;
            fb[bx   + (by+1) * FBW] = c;
            fb[bx+1 + (by+1) * FBW] = c;
            fb[bx+2 + (by+1) * FBW] = c;
            fb[bx+3 + (by+1) * FBW] = c;
            fb[bx+4 + (by+1) * FBW] = c;
            fb[bx+5 + (by+1) * FBW] = c;
            fb[bx   + (by+2) * FBW] = c;
            fb[bx+1 + (by+2) * FBW] = c;
            fb[bx+2 + (by+2) * FBW] = c;
            fb[bx+3 + (by+2) * FBW] = c;
            fb[bx+4 + (by+2) * FBW] = c;
            fb[bx+5 + (by+2) * FBW] = c;
            fb[bx   + (by+3) * FBW] = c;
            fb[bx+1 + (by+3) * FBW] = c;
            fb[bx+2 + (by+3) * FBW] = c;
            fb[bx+3 + (by+3) * FBW] = c;
            fb[bx+4 + (by+3) * FBW] = c;
            fb[bx+5 + (by+3) * FBW] = c;
            fb[bx   + (by+4) * FBW] = c;
            fb[bx+1 + (by+4) * FBW] = c;
            fb[bx+2 + (by+4) * FBW] = c;
            fb[bx+3 + (by+4) * FBW] = c;
            fb[bx+4 + (by+4) * FBW] = c;
            fb[bx+5 + (by+4) * FBW] = c;
            fb[bx   + (by+5) * FBW] = c;
            fb[bx+1 + (by+5) * FBW] = c;
            fb[bx+2 + (by+5) * FBW] = c;
            fb[bx+3 + (by+5) * FBW] = c;
            fb[bx+4 + (by+5) * FBW] = c;
            fb[bx+5 + (by+5) * FBW] = c;
         }
   fbSet(fb);
}
#endif

// draw a glyph g (index into alnum or -1 for space) with fg color at p
// with pixel size s (small font is 2, large one 5), return actual pixel width
// 
// some performance numbers (tested in a loop of 100 repetitions):
// - drawing all glyphs once with size 3 takes ~0.5ms, that is about
//   0.01-0.02ms per glyph
// - size 5 glyphs take ~0.9ms for all glyphs
// - size 6 glyphs tale 1.1ms
// - size 6 with fbFillRect takes about 5ms for all glyphs
int drawGlyph(GPoint p, int8_t g, uint8_t s, uint8_t c) {
   // I have a feeling that this is quite enough... maybe a tad too much
   switch (s) {
#ifdef GLYPH_SIZE_1
      case 1:
         drawGlyph1(p, g, c);
         break;
#endif
      case 2:
         drawGlyph2(p, g, c);
         break;
#ifdef GLYPH_SIZE_3
      case 3:
         drawGlyph3(p, g, c);
         break;
#endif
#ifdef GLYPH_SIZE_4
      case 4:
         drawGlyph4(p, g, c);
         break;
#endif
      case 5:
         drawGlyph5(p, g, c);
         break;
#ifdef GLYPH_SIZE_6
      case 6:
         drawGlyph6(p, g, c);
         break;
#endif
      default:
         for (int x = 0; x < 5; x++)
            for (int y = 0; y < 5; y++)
               if (alnum[g].mask[x+y*5] == '1')
                  fbFillRect(GRect(p.x+x*s, p.y+y*s, s, s), c);
   }
   return alnum[g].w * s;
}

