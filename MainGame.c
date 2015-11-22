#include "MainGame.h"

static SDL_Window* window = NULL;
static int screenWidth = 1024;
static int screenHeight = 768;

void MainGame_run() {
    MainGame_initSystems();
}

void MainGame_initSystems() {
    SDL_Init(SDL_INIT_EVERYTHING);
    window = SDL_CreateWindow("Game Engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenWidth, screenHeight, SDL_WINDOW_OPENGL);
}
