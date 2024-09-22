#ifndef SCREEN_H
#define SCREEN_H

#define NTSC_SCANLINES_TOTAL 262
#define NTSC_SCANLINES_RENDERED 240
#define NTSC_SCANLINE_WIDTH_TOTAL 341
#define NTSC_SCANLINE_WIDTH_RENDERED 256

typedef struct screen *Screen;

Screen create_screen();
void screen_draw_frame(Screen);
void screen_pixel(Screen, Uint8, Uint8, Uint8, int, int);


#endif