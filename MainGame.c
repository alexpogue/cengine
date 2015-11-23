#include "MainGame.h"

#include <stdio.h>
#include "GL/glew.h"
#include "GL/gl.h"

static SDL_Window* window = NULL;
static int screenWidth = 640;
static int screenHeight = 480;

enum GameState {
    GAMESTATE_PLAY,
    GAMESTATE_EXIT
};

static int gameState = GAMESTATE_PLAY;

void fatalError(char* errorString) {
    puts(errorString);
    puts("Enter any key to quit...");
    getchar();
    SDL_Quit();
}

void initSystems() {
    SDL_Init(SDL_INIT_EVERYTHING);
    window = SDL_CreateWindow("Game Engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenWidth, screenHeight, SDL_WINDOW_OPENGL);
    if(window == NULL ) {
        fatalError("SDL Window could not be created!");
    }

    SDL_GLContext glContext = SDL_GL_CreateContext(window);
    if(glContext == NULL) {
        fatalError("SDL_GL context could not be created!");
    }

    GLenum error = glewInit();
    if(error != GLEW_OK) {
        fatalError("Could not initialize glew!");
    }

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    glClearColor(0.0f, 0.0f, 1.0f, 1.0);
}

void processInput() {
    SDL_Event e;
    while(SDL_PollEvent(&e)) {
        switch(e.type) {
            case SDL_QUIT:
                gameState = GAMESTATE_EXIT;
                break;
            case SDL_MOUSEMOTION:
                printf("%d %d\n", e.motion.x, e.motion.y);
                break;
        }
    }
}

void drawGame() {
    glClearDepth(1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnableClientState(GL_COLOR_ARRAY);
    glBegin(GL_TRIANGLES);
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex2f(0, 0);
    glVertex2f(0, 500);
    glVertex2f(500, 500);

    glEnd();

    SDL_GL_SwapWindow(window);
}

void gameLoop() {
    while(gameState != GAMESTATE_EXIT) {
        processInput();
        drawGame();
    }
}

void MainGame_run() {
    initSystems();
    gameLoop();
}
