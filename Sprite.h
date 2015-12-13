#ifndef SPRITE_H
#define SPRITE_H

typedef struct sprite sprite_t;

sprite_t* Sprite_new();
void Sprite_init(sprite_t* sprite, float x, float y, float width, float height);
void Sprite_draw(sprite_t* sprite);
void Sprite_free(sprite_t* sprite);

#endif
