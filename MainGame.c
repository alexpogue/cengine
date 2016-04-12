#include "MainGame.h"
#include "Sprite.h"
#include "Errors.h"
#include "GlslProgram.h"
#include "SDL.h"
#include "GL/glew.h"
#include "GL/gl.h"
#include <stdio.h>

static SDL_Window *window = NULL;
static int screenWidth = 640;
static int screenHeight = 480;
static sprite_t *sprite = NULL;
static glsl_program_t *colorProgram = NULL;
static SDL_GLContext glContext = 0;

enum GameState { GAMESTATE_PLAY, GAMESTATE_EXIT };

static int gameState = GAMESTATE_PLAY;

void initGame(void);
void initSystems(void);
void initShaders(void);
void gameLoop(void);
void processInput(void);
void drawGame(void);
void freeGame(void);
void freeSystems(void);
void freeShaders(void);

void MainGame_run(void)
{
    initGame();
    gameLoop();
    freeGame();
}

void initGame(void)
{
    initSystems();
    sprite = Sprite_new();
    Sprite_init(sprite, -1.0f, -1.0f, 1.0f, 1.0f);
}

void initSystems(void)
{
    GLenum error;

    SDL_Init(SDL_INIT_EVERYTHING);
    window = SDL_CreateWindow("Game Engine",
                              SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED,
                              screenWidth,
                              screenHeight,
                              SDL_WINDOW_OPENGL);
    if (window == NULL ) {
        Errors_fatal("SDL Window could not be created!");
    }
    glContext = SDL_GL_CreateContext(window);
    if (glContext == NULL) {
        Errors_fatal("SDL_GL context could not be created!");
    }
    error = glewInit();
    if (error != GLEW_OK) {
        Errors_fatal("Could not initialize glew!");
    }
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    glClearColor(0.0f, 0.0f, 1.0f, 1.0f);

    initShaders();
}

void initShaders(void)
{
    colorProgram = GlslProgram_new();
    GlslProgram_compileShaders(colorProgram,
                               "shaders/colorShading.vert",
                               "shaders/colorShading.frag");
    GlslProgram_addAttribute(colorProgram, "vertexPosition");
    GlslProgram_linkShaders(colorProgram);
}

void gameLoop(void)
{
    while (gameState != GAMESTATE_EXIT) {
        processInput();
        drawGame();
    }
}

void processInput(void)
{
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        switch (e.type) {
        case SDL_QUIT:
            gameState = GAMESTATE_EXIT;
            break;
        case SDL_MOUSEMOTION:
            printf("%d %d\n", e.motion.x, e.motion.y);
            break;
        }
    }
}

void drawGame(void)
{
    glClearDepth(1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    GlslProgram_use(colorProgram);
    Sprite_draw(sprite);
    GlslProgram_unuse(colorProgram);
    SDL_GL_SwapWindow(window);
}

void freeGame(void)
{
    Sprite_free(sprite);
    freeSystems();
}

void freeSystems(void)
{
    freeShaders();
    SDL_GL_DeleteContext(glContext);
    if (window != NULL) {
        SDL_DestroyWindow(window);
    }
    SDL_Quit();
}

void freeShaders(void)
{
    GlslProgram_free(colorProgram);
}
