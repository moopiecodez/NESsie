#include <SDL2/SDL.h>
#include <stdbool.h>
#include "screen.h"

void handle_events(bool *);

uint8_t dummypat[0x10] = {  0x18, 0x38, 0x18, 0x18, 0x18, 0x18, 0x7E, 0x00, 
                            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
uint8_t *ptr = dummypat;

uint8_t *get_tile(uint8_t* index);

int main(void) {
    Uint32 init_flags = SDL_INIT_VIDEO;
    SDL_Init(init_flags);

    int i;
    int j;

    uint8_t plane1[8][8];
    uint8_t plane2[8][8];
    uint8_t result[8][8];
    for(i = 0; i < 8; i++) {
        for(j = 0; j < 8; j++) {
        plane1[i][j] = *(ptr + i) >> (7-j) & ~(~0<<1); //7-j is needed to get it in correct position in array which is revers of bit position
        plane2[i][j] = *(ptr + i + 8) >> (7-j) & ~(~0<<1);
        result[i][j] = plane1[i][j] + plane2[i][j];
        printf("%d", result[i][j]);
        }    
    }  

    Screen screen = create_screen();

    int clock = 0;
    bool quit = false;

    while (!quit) {
        handle_events(&quit);
        //update game state
        for(i = 0; i < 8; i++) {
            for(int j = 0; j < 8; j++) {
                uint8_t pixel_on = result[i][j];
                printf("%d", pixel_on);
                if(pixel_on != 0){
                    screen_pixel(screen, 0, 0, 255, j, i);
                }
            }
            printf("\n");
        }
        // for(int i = 0; i < NTSC_SCANLINES_RENDERED; i++) {
        //     screen_pixel(screen, 0, 0, 255, i, i);
        // }
        // draw current frame
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