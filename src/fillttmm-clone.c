#include <pebble.h>

#include "glyphs.h"
#include "fb.h"
#include "util.h"

#if 1
#undef APP_LOG
#define APP_LOG(...)
#define START_TIME_MEASURE() {
#define END_TIME_MEASURE(x) }
#else
static unsigned int get_time(void) {
   time_t s;
   uint16_t ms;
   time_ms(&s, &ms);
   return (s & 0xfffff) * 1000 + ms;
}

#define START_TIME_MEASURE() \
   {                         \
   unsigned tm_0 = get_time()
#define END_TIME_MEASURE(x)                                       \
   unsigned tm_1 = get_time();                                    \
   APP_LOG(APP_LOG_LEVEL_DEBUG, "%s: took %dms", x, tm_1 - tm_0); \
   }
#endif

struct App {
   Window *w;
   uint8_t color;
   struct tm t;
};

struct App *g;

static int drawString(GPoint p, char *s, int size, uint8_t c) {
   int x = 0;
   for (int i = 0; s[i]; i++)
      x += drawGlyph(GPoint(p.x+x, p.y), char2index(s[i]), size, c) + size + 1;
   return x;
}

static int drawNumber(GPoint p, unsigned num, int s, uint8_t c) {
   char buf[10];
   snprintf(buf, sizeof(buf), "%u", num);
   return drawString(p, buf, s, c);
}

static void draw(void) {
   START_TIME_MEASURE();

   static uint8_t bg = 0xf0; // red
   static uint8_t fg = 0xff; // white
   fbClear(bg);

   static char *weekday[] = {
      "sunday",
      "monday",
      "tuesday",
      "wednesday",
      "thursday",
      "friday",
      "saturday"
   };

   static char *month[] = {
      "jan",
      "feb",
      "mar",
      "apr",
      "may",
      "jun",
      "jul",
      "aug",
      "sep",
      "oct",
      "nov",
      "dec"
   };

   GPoint p = { 12, 17 };
   switch (g->t.tm_min & 1) {
      case 0: // day-of-month 3-letter-month
         p.x += drawNumber(p, g->t.tm_mday, 2, fg);
         p.x += 14;
         p.x += drawString(p, month[g->t.tm_mon], 2, fg);
         break;
      case 1: // weekday
         drawString(p, weekday[g->t.tm_wday], 2, fg);
         break;
   }

   p = (GPoint){ 12, 46 };
   drawNumber(p, g->t.tm_hour, 5, fg);

   int min = g->t.tm_min;
   p = (GPoint) { 14, 89 };
   for (int l = 0; l < 6; l++) {
      int pl = p.y + l * 13;
      for (int c = 0; c < 10; c++) {
         int pc = p.x + c * 13;
         int on = l * 10 + c < min ? 3 : 1;
         fbFillRect(GRect(pc - on, pl - on, on * 2, on * 2), fg);
      }
   }

   END_TIME_MEASURE("drawing");
}

static void update(Layer *layer, GContext *ctx) {
   GBitmap *bmp = graphics_capture_frame_buffer(ctx);
   fbSet(gbitmap_get_data(bmp));

   draw();

   graphics_release_frame_buffer(ctx, bmp);
   fbSet(NULL);
}

static void tick(struct tm *tick_time, TimeUnits units_changed) {
   g->t = *tick_time;
   layer_mark_dirty(window_get_root_layer(g->w));
}

static void window_load(Window *w) {
   layer_set_update_proc(window_get_root_layer(w), update);
}

static void window_unload(Window *w) {
}

static void init_time(struct App *a) {
   time_t t = time(NULL);
   struct tm *tm = localtime(&t);
   a->t = *tm;
}

static void init(struct App *a) {
   g = a;
   a->w = window_create();
   init_time(a);
   tick_timer_service_subscribe(MINUTE_UNIT, tick);
   window_set_user_data(a->w, a);
   window_set_window_handlers(a->w,
                              (WindowHandlers){
                                 .load = window_load, .unload = window_unload,
                              });
   window_stack_push(a->w, false);
}

static void fini(struct App *a) {
   tick_timer_service_unsubscribe();
   window_destroy(a->w);
   a->w = NULL;
   g = NULL;
}

int main(void) {
   struct App a;
   memset(&a, 0, sizeof(a));
   init(&a);
   app_event_loop();
   fini(&a);
   return 0;
}
