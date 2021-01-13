#ifndef VBOSPHERE_H
#define VBOSPHERE_H

#include "drawable.h"
#include "cookbookogl.h"
#include <glm/glm.hpp>

class VBOSphere : public Drawable
{
private:
    unsigned int vaoHandle;
    GLuint nVerts, elements;
	float radius;
	GLuint slices, stacks;

    void generateVerts(float * , float * ,float *, GLuint *);

public:
    glm::vec3 color;
    
    VBOSphere(float, GLuint, GLuint, glm::vec3);

    void render() const;

    int getVertexArrayHandle();
};

#endif // VBOSPHERE_H
