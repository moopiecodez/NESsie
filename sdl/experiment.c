#include <SDL2/SDL.h>
#include <stdbool.h>

#define NTSC_SCANLINES_TOTAL 262
#define NTSC_SCANLINES_RENDERED 240
#define NTSC_SCANLINE_WIDTH_TOTAL 341
#define NTSC_SCANLINE_WIDTH_RENDERED 256
#define MODERN_WIDTH 1280
#define MODERN_HEIGHT 960

SDL_Window *create_screen();
void draw();
void handle_events(bool *);

int main(void) {
    Uint32 flags;
    bool quit;

    flags = SDL_INIT_VIDEO;
    SDL_Init(flags);
    // SDL_Window *Nessie_screen = create_screen();
    create_screen();

    quit = false;
    int clock = 0;
    while (!quit) {
        printf("Clock cycle: %d\n", clock);
        handle_events(&quit);
        draw();
        clock++;
    }

    SDL_Quit();
    return 0;
}

SDL_Window *create_screen() {
    SDL_Window *screen;
    char *title = "Opening to Nessie's soul";
    int screen_pos_x = SDL_WINDOWPOS_CENTERED;
    int screen_pos_y = SDL_WINDOWPOS_CENTERED;
    int screen_width = NTSC_SCANLINE_WIDTH_TOTAL;
    int screen_height = NTSC_SCANLINES_TOTAL;
    Uint32 screen_flags = 0;

    screen = SDL_CreateWindow(
        title, screen_pos_x, screen_pos_y,
        screen_width, screen_height, screen_flags);
    return screen;
}

void draw() {
    printf("I drawd\n");
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