#ifndef BMP_H
#define BMP_H

#include "shader_m.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

typedef struct {
    unsigned int width;
    unsigned int height;
} ImageSize;

class BMPRenderer {
private:
    unsigned int vao;
    unsigned int vbo, ebo;
    ImageSize size;
    Shader shader;
    GLuint id;
    GLuint loadBMP(const char*);
    float *generateVerts(void);
    unsigned int *generateIndices(void);
    void setShader(Shader);
public:
    unsigned int getVertexArrayHandle();
    void render(unsigned int, unsigned int);
    BMPRenderer(const char*, Shader);
};

#endif