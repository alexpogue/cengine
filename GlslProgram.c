#include "GlslProgram.h"
#include "Errors.h"
#include "GL/glew.h"
#include <stdlib.h>
#include <stdio.h>

struct glsl_program {
    GLuint programID;
    GLuint vertexShaderID;
    GLuint fragmentShaderID;
    int numAttributes;
};

glsl_program_t *GlslProgram_new()
{
    glsl_program_t *program = malloc(sizeof(glsl_program_t));
    if (program == NULL) {
        Errors_fatal("malloc failed in GlslProgram_new()");
    }
    program->programID = 0;
    program->vertexShaderID = 0;
    program->fragmentShaderID = 0;
    program->numAttributes = 0;
    return program;
}

ssize_t getFileContents(char **buf, char *filePath)
{
    FILE *fp = NULL;
    long fileSize;
    if (buf == NULL || filePath == NULL) {
        Errors_fatal("Null arguments to getFileContents");
    }
    fp = fopen(filePath, "r");
    if (fp == NULL) {
        Errors_fatal("Could not open file in getFileContents");
    }
    fseek(fp, 0, SEEK_END);
    fileSize = ftell(fp);
    if (fileSize == -1) {
        fclose(fp);
        Errors_fatal("Could not get file size in getFileContents");
    }
    rewind(fp);
    *buf = realloc(*buf, fileSize + 1);
    if (*buf == NULL) {
        fclose(fp);
        Errors_fatal("Realloc failed in getFileContents");
    }
    fread(*buf, 1, fileSize, fp);
    (*buf)[fileSize] = 0;
    fclose(fp);
    return fileSize;
}

void GlslProgram_use(glsl_program_t *program)
{
    int i;
    glUseProgram(program->programID);
    for (i = 0; i < program->numAttributes; i++) {
        glEnableVertexAttribArray(i);
    }
}

void GlslProgram_unuse(glsl_program_t *program)
{
    int i;
    glUseProgram(0);
    for (i = 0; i < program->numAttributes; i++) {
        glDisableVertexAttribArray(i);
    }
}

void compileShader(GLuint id, char *filePath)
{
    char *fileContents = NULL;
    GLint success = 0;
    ssize_t charsRead = getFileContents(&fileContents, filePath);
    if (charsRead == -1) {
        Errors_fatal("File read failed. Couldn't compile shader.");
    }
    glShaderSource(id, 1, (const char **)&fileContents, NULL);
    glCompileShader(id);
    glGetShaderiv(id, GL_COMPILE_STATUS, &success);
    if (success == GL_FALSE) {
        char *errorLog = NULL;
        GLint maxLength = 0;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &maxLength);
        errorLog = malloc(maxLength);
        if (errorLog == NULL) {
            Errors_fatal("malloc failed in compileShader.");
        }
        glGetShaderInfoLog(id, maxLength, &maxLength, errorLog);

        glDeleteShader(id);

        puts(errorLog);
        free(errorLog);
        free(fileContents);
        Errors_fatal("Shader failed to compile");
    }
    free(fileContents);
}

void GlslProgram_compileShaders(glsl_program_t *program,
                                char *vertexShaderFilePath,
                                char *fragmentShaderFilePath)
{
    if (program == NULL ||
        vertexShaderFilePath == NULL ||
        fragmentShaderFilePath == NULL) {
        Errors_fatal("Null arguments to GlslProgram_compileShaders");
    }
    program->programID = glCreateProgram();
    program->vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    if (program->vertexShaderID == 0) {
        Errors_fatal("Vertex shader failed to be created!");
    }
    program->fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
    if (program->fragmentShaderID == 0) {
        Errors_fatal("Fragment shader failed to be created!");
    }
    compileShader(program->vertexShaderID, vertexShaderFilePath);
    compileShader(program->fragmentShaderID, fragmentShaderFilePath);
}

void GlslProgram_linkShaders(glsl_program_t *program)
{
    GLuint programID;
    GLint isLinked;
    /* Vertex and fragment shaders are successfully compiled.
       Now time to link them together into a program.
       Get a program object. */
    programID = program->programID;

    /* Attach our shaders to our program */
    glAttachShader(programID, program->vertexShaderID);
    glAttachShader(programID, program->fragmentShaderID);

    /* Link our program */
    glLinkProgram(programID);

    /* Note the different functions: glGetProgram* instead of glGetShader*. */
    isLinked = 0;
    glGetProgramiv(programID, GL_LINK_STATUS, (int *)&isLinked);
    if (isLinked == GL_FALSE) {
        GLint maxLength = 0;
        glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &maxLength);

        if (maxLength != 0) {
            /* The maxLength includes the NULL character */
            char *errorLog = malloc(maxLength);
            glGetProgramInfoLog(programID, maxLength, &maxLength, errorLog);
            puts(errorLog);
            free(errorLog);
            Errors_fatal("Shader failed to link");
        } else {
            Errors_fatal("Shader failed to link, no information log found");
        }

        /* We don't need the program anymore. */
        glDeleteProgram(programID);
        /* Don't leak shaders either. */
        glDeleteShader(program->vertexShaderID);
        glDeleteShader(program->fragmentShaderID);
        Errors_fatal("Shaders failed to link!");
    }

    /* Always detach shaders after a successful link. */
    glDetachShader(program->programID, program->vertexShaderID);
    glDetachShader(program->programID, program->fragmentShaderID);

    glDeleteShader(program->vertexShaderID);
    glDeleteShader(program->fragmentShaderID);
}

void GlslProgram_addAttribute(glsl_program_t *program, char *name)
{
    if (program == NULL) {
        Errors_fatal("program is NULL in GlslProgram_addAttribute()");
    }
    glBindAttribLocation(program->programID, program->numAttributes++, name);
}

void GlslProgram_free(glsl_program_t *program)
{
    if (program != NULL) {
        glDeleteProgram(program->programID);
        glDeleteShader(program->vertexShaderID);
        glDeleteShader(program->fragmentShaderID);
        free(program);
        program = NULL;
    }
}
