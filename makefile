all: cengine

cengine: main.o MainGame.o
	gcc -I/usr/include/SDL2 -o cengine main.o MainGame.o -lSDL2 -lGL -lGLEW

main.o: main.c MainGame.h
	gcc -I/usr/include/SDL2 -c main.c 

MainGame.o: MainGame.c MainGame.h
	gcc -I/usr/include/SDL2 -c MainGame.c

clean:
	rm *.o
