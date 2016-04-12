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

void getRectangleVertices(float *vertices, float x, float y, float w, float h);

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
    float vertices[12];
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

    getRectangleVertices(vertices, x, y, width, height);

    glBindBuffer(GL_ARRAY_BUFFER, sprite->vboID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0); /* unbind buffer */
}

void getRectangleVertices(float *vertices, float x, float y, float w, float h)
{
    /* first triangle */
    /* top-right */
    vertices[0] = x + w;
    vertices[1] = y + h;
    /* top-left */
    vertices[2] = x;
    vertices[3] = y + h;
    /* bottom-left */
    vertices[4] = x;
    vertices[5] = y;

    /* second triangle */
    /* bottom-left */
    vertices[6] = x;
    vertices[7] = y;
    /* bottom-right */
    vertices[8] = x + w;
    vertices[9] = y;
    /* top-right */
    vertices[10] = x + w;
    vertices[11] = y + h;
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
