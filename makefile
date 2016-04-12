all: cengine

cengine: main.o MainGame.o Sprite.o Errors.o GlslProgram.o
	gcc -g -Wall -Wextra -Wshadow -Wpointer-arith -Wcast-qual -Wstrict-prototypes -Wmissing-prototypes -pedantic -o cengine main.o MainGame.o Sprite.o Errors.o GlslProgram.o -lSDL2 -lGL -lGLEW

main.o: main.c MainGame.h
	gcc -g -Wall -Wextra -Wshadow -Wpointer-arith -Wcast-qual -Wstrict-prototypes -Wmissing-prototypes -pedantic -I/usr/include/SDL2 -c main.c

MainGame.o: MainGame.c MainGame.h
	gcc -g -Wall -Wextra -Wshadow -Wpointer-arith -Wcast-qual -Wstrict-prototypes -Wmissing-prototypes -pedantic -I/usr/include/SDL2 -c MainGame.c

Errors.o: Errors.c Errors.h
	gcc -g -Wall -Wextra -Wshadow -Wpointer-arith -Wcast-qual -Wstrict-prototypes -Wmissing-prototypes -pedantic -I/usr/include/SDL2 -c Errors.c

GlslProgram.o: GlslProgram.c GlslProgram.h
	gcc -g -Wall -Wextra -Wshadow -Wpointer-arith -Wcast-qual -Wstrict-prototypes -Wmissing-prototypes -pedantic -c GlslProgram.c

Sprite.o: Sprite.c Sprite.h
	gcc -g -Wall -Wextra -Wshadow -Wpointer-arith -Wcast-qual -Wstrict-prototypes -Wmissing-prototypes -pedantic -c Sprite.c

clean:
	rm *.o
