#include "Sprite.h"
#include "Errors.h"
#include "GL/glew.h"
#include <stdlib.h>

struct sprite {
    float x;
    float y;
    float width;
    float height;
    GLuint vboID;
};

sprite_t *Sprite_new()
{
    sprite_t *sprite = malloc(sizeof(sprite_t));
    if (sprite == NULL) {
        Errors_fatal("malloc failed in Sprite_new()");
    }
    sprite->vboID = 0;
    return sprite;
}

void Sprite_init(sprite_t *sprite, float x, float y, float width, float height)
{
    float vertData[12];
    if (sprite == NULL) {
        Errors_fatal("sprite is NULL in Sprite_init()");
    }

    sprite->x = x;
    sprite->y = y;
    sprite->width = width;
    sprite->height = height;

    if (sprite->vboID == 0) {
        glGenBuffers(1, &(sprite->vboID));
    }

    /* first triangle */
    /* top-right */
    vertData[0] = x + width;
    vertData[1] = y + height;
    /* top-left */
    vertData[2] = x;
    vertData[3] = y + height;
    /* bottom-left */
    vertData[4] = x;
    vertData[5] = y;

    /* second triangle */
    /* bottom-left */
    vertData[6] = x;
    vertData[7] = y;
    /* bottom-right */
    vertData[8] = x + width;
    vertData[9] = y;
    /* top-right */
    vertData[10] = x + width;
    vertData[11] = y + height;

    glBindBuffer(GL_ARRAY_BUFFER, sprite->vboID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertData), vertData, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0); /* unbind buffer */
}

void Sprite_draw(sprite_t *sprite)
{
    if (sprite == NULL) {
        Errors_fatal("sprite is NULL in Sprite_draw()");
    }
    glBindBuffer(GL_ARRAY_BUFFER, sprite->vboID);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glDisableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0); /* unbind buffer */
}

void Sprite_free(sprite_t *sprite)
{
    if (sprite && sprite->vboID != 0) {
        glDeleteBuffers(1, &(sprite->vboID));
    }
    if (sprite) {
        free(sprite);
    }
    sprite = NULL;
}
