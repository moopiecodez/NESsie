#include <SDL2/SDL.h>

SDL_Window *create_screen();

int main(void) {
    Uint32 flags;
    flags = SDL_INIT_VIDEO;

    SDL_Init(flags);
    SDL_Window *Nessie_screen = create_screen();

    SDL_Delay(3000);
    SDL_Quit();

    return 0;
}

SDL_Window *create_screen() {
    SDL_Window *screen;
    char *title = "Opening to Nessie's soul";
    int screen_pos_x = SDL_WINDOWPOS_CENTERED;
    int screen_pos_y = SDL_WINDOWPOS_CENTERED;
    int screen_width = 1280;
    int screen_height = 960;
    Uint32 screen_flags = 0;

    screen = SDL_CreateWindow(
        title, screen_pos_x, screen_pos_y,
        screen_width, screen_height, screen_flags);
    return screen;
}