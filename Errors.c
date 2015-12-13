#include "Errors.h"
#include "SDL.h"
#include <stdio.h>

void Errors_fatal(char *errorString) {
    if (errorString != NULL) {
        puts(errorString);
    }
    puts("Enter any key to quit...");
    getchar();
    SDL_Quit();
}
