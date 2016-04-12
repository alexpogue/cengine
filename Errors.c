#include "Errors.h"
#include "SDL.h"
#include <stdio.h>
#include <stdlib.h>

void Errors_fatal(char *errorString)
{
    if (errorString != NULL) {
        puts(errorString);
    }
    puts("Enter any key to quit...");
    getchar();
    SDL_Quit();
    exit(1);
}
