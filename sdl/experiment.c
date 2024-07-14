#include <SDL2/SDL.h>

int main(void) {
    Uint32 flags;
    flags = SDL_INIT_EVERYTHING;

    SDL_Init(flags);
    SDL_Quit();

    return 0;
}