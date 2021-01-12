#ifndef PDBREADER_H
#define PDBREADER_H

#include "shader_m.h"
#include "camera.h"
#include <glm/glm.hpp>
#include <vector>
#include <string>

using namespace std;

class Point {
public:
    float x, y, z;
    string toString();
    glm::vec3 toVec3();
};

class Atom {
public:
    Point center;
    float radius;
    char  name[3];
    string toString();
};

class Molecule {
public:
    vector<Atom> atoms;
    Point min, max;
    string toString();
    Molecule fromPDB(char *file);
    Camera resetCamera();
    void render_vanderWalls(Shader shader, Camera camera, const int SCR_WIDTH, const int SCR_HEIGHT) const;
};

int getMoleculeFromPDB(char *file, Molecule &molecule);

#endif