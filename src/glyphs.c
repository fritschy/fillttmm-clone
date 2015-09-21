#include "glyphs.h"
#include "fb.h"

#define GWIDTH 7
#define GHEIGHT 5

struct glyph {
   uint8_t w; // actual glyph width in pels
   uint8_t mask[GWIDTH*GHEIGHT+1]; // pixels we actually fill with fg color
};

// using ascii chars here, for maintainability
struct glyph const alnum[10+26+1] = {
   { 5, "1111100" // 0
        "1000100"
        "1000100"
        "1000100"
        "1111100" },
   { 2, "1100000" // 1
        "0100000"
        "0100000"
        "0100000"
        "0100000" },
   { 5, "1111100" // 2
        "0000100"
        "1111100"
        "1000000"
        "1111100" },
   { 5, "1111100" // 3
        "0000100"
        "0111100"
        "0000100"
        "1111100" },
   { 5, "1000100" // 4
        "1000100"
        "1111100"
        "0000100"
        "0000100" },
   { 5, "1111100" // 5
        "1000000"
        "1111100"
        "0000100"
        "1111100" },
   { 5, "1111100" // 6
        "1000000"
        "1111100"
        "1000100"
        "1111100" },
   { 5, "1111100" // 7
        "0000100"
        "0000100"
        "0000100"
        "0000100" },
   { 5, "1111100" // 8
        "1000100"
        "1111100"
        "1000100"
        "1111100" },
   { 5, "1111100" // 9
        "1000100"
        "1111100"
        "0000100"
        "1111100" },

   { 5, "1111100" // A
        "1000100"
        "1111100"
        "1000100"
        "1000100" },
   { 5, "1111000" // B
        "1000100"
        "1111100"
        "1000100"
        "1111000" },
   { 4, "1111000" // C
        "1000000"
        "1000000"
        "1000000"
        "1111000" },
   { 5, "1111000" // D
        "1000100"
        "1000100"
        "1000100"
        "1111000" },
   { 5, "1111100" // E
        "1000000"
        "1111000"
        "1000000"
        "1111100" },
   { 5, "1111100" // F
        "1000000"
        "1111000"
        "1000000"
        "1000000" },
   { 5, "1111100" // G
        "1000000"
        "1001100"
        "1000100"
        "1111100" },
   { 5, "1000100" // H
        "1000100"
        "1111100"
        "1000100"
        "1000100" },
   { 1, "1000000" // I
        "1000000"
        "1000000"
        "1000000"
        "1000000" },
   { 4, "1111000" // J
        "0001000"
        "0001000"
        "1001000"
        "0110000" },
   { 4, "1001000" // K
        "1010000"
        "1100000"
        "1010000"
        "1001000" },
   { 4, "1000000" // L
        "1000000"
        "1000000"
        "1000000"
        "1111000" },
   { 7, "1111111" // M
        "1001001"
        "1001001"
        "1001001"
        "1001001" },
   { 5, "1111100" // N
        "1000100"
        "1000100"
        "1000100"
        "1000100" },
   { 5, "1111100" // O
        "1000100"
        "1000100"
        "1000100"
        "1111100" },
   { 5, "1111100" // P
        "1000100"
        "1111100"
        "1000000"
        "1000000" },
   { 5, "1111100" // Q
        "1000100"
        "1000100"
        "1001100"
        "1111100" },
   { 5, "1111000" // R
        "1001000"
        "1111000"
        "1010000"
        "1001000" },
   { 5, "1111100" // S
        "1000000"
        "1111100"
        "0000100"
        "1111100" },
   { 5, "1111100" // T
        "0010000"
        "0010000"
        "0010000"
        "0010000" },
   { 5, "1000100" // U
        "1000100"
        "1000100"
        "1000100"
        "1111100" },
   { 5, "1000100" // V
        "1000100"
        "0101000"
        "0101000"
        "0010000" },
   { 5, "1000100" // W
        "1000100"
        "1010100"
        "1101100"
        "1000100" },
   { 5, "1000100" // X
        "0101000"
        "0010000"
        "0101000"
        "1000100" },
   { 5, "1000100" // Y
        "1000100"
        "0111000"
        "0010000"
        "0010000" },
   { 5, "1111100" // Z
        "0001000"
        "0010000"
        "0100000"
        "1111100" },

   { 12,"0000000" // space
        "0000000"
        "0000000"
        "0000000"
        "0000000" }
};

#ifdef GLYPH_SIZE_1
static void drawGlyph1(GPoint p, int8_t g, uint8_t c) {
   uint8_t *fb = fbSet(NULL);
   for (int x = 0; x < GWIDTH; x++)
      for (int y = 0; y < GHEIGHT; y++)
         if (alnum[g].mask[x+y*GWIDTH] == '1')
            fb[p.x+x + (p.y+y) * FBW] = c;
   fbSet(fb);
}
#endif

static void drawGlyph2(GPoint p, int8_t g, uint8_t c) {
   uint8_t *fb = fbSet(NULL);
   for (int x = 0; x < GWIDTH; x++)
      for (int y = 0; y < GHEIGHT; y++)
         if (alnum[g].mask[x+y*GWIDTH] == '1') {
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
   for (int x = 0; x < GWIDTH; x++)
      for (int y = 0; y < GHEIGHT; y++)
         if (alnum[g].mask[x+y*GWIDTH] == '1') {
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
   for (int x = 0; x < GWIDTH; x++)
      for (int y = 0; y < GHEIGHT; y++)
         if (alnum[g].mask[x+y*GWIDTH] == '1') {
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
   for (int x = 0; x < GWIDTH; x++)
      for (int y = 0; y < GHEIGHT; y++)
         if (alnum[g].mask[x+y*GWIDTH] == '1') {
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
   for (int x = 0; x < GWIDTH; x++)
      for (int y = 0; y < GHEIGHT; y++)
         if (alnum[g].mask[x+y*GWIDTH] == '1') {
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
         for (int x = 0; x < GWIDTH; x++)
            for (int y = 0; y < GHEIGHT; y++)
               if (alnum[g].mask[x+y*GWIDTH] == '1')
                  fbFillRect(GRect(p.x+x*s, p.y+y*s, s, s), c);
   }
   return alnum[g].w * s;
}

