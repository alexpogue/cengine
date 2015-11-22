#include <stdio.h>
#include "MainGame.h"

int main(int argc, char** argv) {
    SDL_Init(SDL_INIT_EVERYTHING);
    MainGame_run();
    puts("press any key to quit");
    getchar();

    return 0;
}
