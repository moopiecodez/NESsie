#include <SDL2/SDL.h>
#include <stdbool.h>
#include "screen.h"

void handle_events(bool *);

int main(void) {
    Uint32 init_flags = SDL_INIT_VIDEO;
    SDL_Init(init_flags);

    Screen screen = create_screen();

    int clock = 0;
    bool quit = false;

    while (!quit) {
        handle_events(&quit);
        //update game state
        for(int i = 0; i < NTSC_SCANLINES_RENDERED; i++) {
            screen_pixel(screen, 0, 0, 255, i, i);
        }
        //draw current frame
        screen_draw_frame(screen);
        clock++;
    }
    printf("Clock cycle: %d\n", clock);

    SDL_Quit();
    return 0;
}

void handle_events(bool *quit) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
            *quit = true;
        }
    }
}

//forloop can't be in this while loop directly as prevents exit
    // int PPU_count;  
    // for(int i = 0; i < NTSC_SCANLINES_TOTAL; i++) {
    //     for(int j = 0; j < NTSC_SCANLINE_WIDTH_TOTAL; j++){
    //         PPU_count++;
    //         printf("Scanline: %d, Pixel: %d, PPU cycles: %d \n", i, j, PPU_count);
    //     }
    // }