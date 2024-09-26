#include <SDL2/SDL.h>
#include "screen.h"

#define TITLE "Portal to Nessie's world"

struct screen {
    SDL_Window *window;
    SDL_Renderer *renderer;
};

Screen create_screen() {
    Screen screen = malloc(sizeof(struct screen));
    int x = SDL_WINDOWPOS_CENTERED;
    int y = SDL_WINDOWPOS_CENTERED;
    int w = NTSC_SCANLINE_WIDTH_RENDERED;
    int h = NTSC_SCANLINES_RENDERED;
    Uint32 screen_flags = 0;
    int index = -1; //driver index default value
    Uint32 renderer_flags = 0;

    screen->window = SDL_CreateWindow(TITLE, x, y, w, h, screen_flags);
    screen->renderer = SDL_CreateRenderer(screen->window, index, renderer_flags);

    return screen;
}

void screen_draw_frame(Screen screen) {
    SDL_RenderPresent(screen->renderer);
}

void screen_pixel(Screen screen, Uint8 r, Uint8 g, Uint8 b, int x, int y) {
    SDL_SetRenderDrawColor(screen->renderer, r, g, b, SDL_ALPHA_OPAQUE);
    SDL_RenderDrawPoint(screen->renderer, x, y);
}
