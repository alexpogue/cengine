#ifndef GLSLPROGRAM_H
#define GLSLPROGRAM_H

typedef struct glsl_program glsl_program_t;

glsl_program_t *GlslProgram_new(void);
void GlslProgram_compileShaders(glsl_program_t *program,
                                char *vertexShaderFilePath,
                                char *fragmentShaderFilePath);
void GlslProgram_linkShaders(glsl_program_t *program);
void GlslProgram_addAttribute(glsl_program_t *program, char *name);
void GlslProgram_use(glsl_program_t *program);
void GlslProgram_unuse(glsl_program_t *program);
void GlslProgram_free(glsl_program_t *program);

#endif
