#include "glyphs.h"
#include "fb.h"

#define GWIDTH 7
#define GHEIGHT 5

struct glyph {
   uint8_t mask[5]; // pixels we actually fill with fg color
   uint8_t w;       // actual glyph width in pels
} __attribute__((packed));

struct glyph const alnum[10+26+1] = {
   { { 0b1111100, // 0
       0b1000100,
       0b1000100,
       0b1000100,
       0b1111100 }, 5 },
   { { 0b1100000, // 1
       0b0100000,
       0b0100000,
       0b0100000,
       0b0100000 }, 2 },
   { { 0b1111100, // 2
       0b0000100,
       0b1111100,
       0b1000000,
       0b1111100 }, 5 },
   { { 0b1111100, // 3
       0b0000100,
       0b0111100,
       0b0000100,
       0b1111100 }, 5 },
   { { 0b1000100, // 4
       0b1000100,
       0b1111100,
       0b0000100,
       0b0000100 }, 5 },
   { { 0b1111100, // 5
       0b1000000,
       0b1111100,
       0b0000100,
       0b1111100 }, 5 },
   { { 0b1111100, // 6
       0b1000000,
       0b1111100,
       0b1000100,
       0b1111100 }, 5 },
   { { 0b1111100, // 7
       0b0000100,
       0b0000100,
       0b0000100,
       0b0000100 }, 5 },
   { { 0b1111100, // 8
       0b1000100,
       0b1111100,
       0b1000100,
       0b1111100 }, 5 },
   { { 0b1111100, // 9
       0b1000100,
       0b1111100,
       0b0000100,
       0b1111100 }, 5 },

   { { 0b1111100, // A
       0b1000100,
       0b1111100,
       0b1000100,
       0b1000100 }, 5 },
   { { 0b1111000, // B
       0b1000100,
       0b1111100,
       0b1000100,
       0b1111000 }, 5 },
   { { 0b1111000, // C
       0b1000000,
       0b1000000,
       0b1000000,
       0b1111000 }, 4 },
   { { 0b1111000, // D
       0b1000100,
       0b1000100,
       0b1000100,
       0b1111000 }, 5 },
   { { 0b1111100, // E
       0b1000000,
       0b1111000,
       0b1000000,
       0b1111100 }, 5 },
   { { 0b1111100, // F
       0b1000000,
       0b1111000,
       0b1000000,
       0b1000000 }, 5 },
   { { 0b1111100, // G
       0b1000000,
       0b1001100,
       0b1000100,
       0b1111100 }, 5 },
   { { 0b1000100, // H
       0b1000100,
       0b1111100,
       0b1000100,
       0b1000100 }, 5 },
   { { 0b1000000, // I
       0b1000000,
       0b1000000,
       0b1000000,
       0b1000000 }, 1 },
   { { 0b1111000, // J
       0b0001000,
       0b0001000,
       0b1001000,
       0b0110000 }, 4 },
   { { 0b1001000, // K
       0b1010000,
       0b1100000,
       0b1010000,
       0b1001000 }, 4 },
   { { 0b1000000, // L
       0b1000000,
       0b1000000,
       0b1000000,
       0b1111000 }, 4 },
   { { 0b1111111, // M
       0b1001001,
       0b1001001,
       0b1001001,
       0b1001001 }, 7 },
   { { 0b1111100, // N
       0b1000100,
       0b1000100,
       0b1000100,
       0b1000100 }, 5 },
   { { 0b1111100, // O
       0b1000100,
       0b1000100,
       0b1000100,
       0b1111100 }, 5 },
   { { 0b1111100, // P
       0b1000100,
       0b1111100,
       0b1000000,
       0b1000000 }, 5 },
   { { 0b1111100, // Q
       0b1000100,
       0b1000100,
       0b1001100,
       0b1111100 }, 5 },
   { { 0b1111000, // R
       0b1001000,
       0b1111000,
       0b1010000,
       0b1001000 }, 5 },
   { { 0b1111100, // S
       0b1000000,
       0b1111100,
       0b0000100,
       0b1111100 }, 5 },
   { { 0b1111100, // T
       0b0010000,
       0b0010000,
       0b0010000,
       0b0010000 }, 5 },
   { { 0b1000100, // U
       0b1000100,
       0b1000100,
       0b1000100,
       0b1111100 }, 5 },
   { { 0b1000100, // V
       0b1000100,
       0b0101000,
       0b0101000,
       0b0010000 }, 5 },
   { { 0b1000100, // W
       0b1000100,
       0b1010100,
       0b1101100,
       0b1000100 }, 5 },
   { { 0b1000100, // X
       0b0101000,
       0b0010000,
       0b0101000,
       0b1000100 }, 5 },
   { { 0b1000100, // Y
       0b1000100,
       0b0111000,
       0b0010000,
       0b0010000 }, 5 },
   { { 0b1111100, // Z
       0b0001000,
       0b0010000,
       0b0100000,
       0b1111100 }, 5 },

   { { 0b0000000, // space
       0b0000000,
       0b0000000,
       0b0000000,
       0b0000000 }, 12 }
};

static inline int mask_bit_set(uint8_t const m[const static 5], int x, int y) {
   return (m[y] >> (GWIDTH-1-x)) & 1;
}

#ifdef GLYPH_SIZE_1
static void drawGlyph1(GPoint p, int8_t g, uint8_t c) {
   uint8_t *fb = fbSet(NULL);
   for (int y = 0; y < GHEIGHT; y++)
      for (int x = 0; x < GWIDTH; x++)
         if (mask_bit_set(alnum[g].mask, x, y))
            fb[p.x+x + (p.y+y) * FBW] = c;
   fbSet(fb);
}
#endif

static void drawGlyph2(GPoint p, int8_t g, uint8_t c) {
   uint8_t *fb = fbSet(NULL);
   for (int y = 0; y < GHEIGHT; y++)
      for (int x = 0, by = p.y+y*2; x < GWIDTH; x++)
         if (mask_bit_set(alnum[g].mask, x, y)) {
            int bx = p.x+x*2;
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
   for (int y = 0; y < GHEIGHT; y++)
      for (int x = 0, by = p.y+y*3; x < GWIDTH; x++)
         if (mask_bit_set(alnum[g].mask, x, y)) {
            int bx = p.x+x*3;
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
   for (int y = 0; y < GHEIGHT; y++)
      for (int x = 0, by = p.y+y*4; x < GWIDTH; x++)
         if (mask_bit_set(alnum[g].mask, x, y)) {
            int bx = p.x+x*4;
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
   for (int y = 0; y < GHEIGHT; y++)
      for (int x = 0, by = p.y+y*5; x < GWIDTH; x++)
         if (mask_bit_set(alnum[g].mask, x, y)) {
            int bx = p.x+x*5;
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
   for (int y = 0; y < GHEIGHT; y++)
      for (int x = 0, by = p.y+y*6; x < GWIDTH; x++)
         if (mask_bit_set(alnum[g].mask, x, y)) {
            int bx = p.x+x*5;
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
         for (int y = 0; y < GHEIGHT; y++)
            for (int x = 0, by = p.y+y*s; x < GWIDTH; x++)
               if (mask_bit_set(alnum[g].mask, x, y))
                  fbFillRect(GRect(p.x+x*s, by, s, s), c);
   }
   return alnum[g].w * s;
}

