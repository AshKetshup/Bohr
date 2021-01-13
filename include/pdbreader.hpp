#ifndef PDBREADER_H
#define PDBREADER_H

#include "shader_m.h"
#include "camera.h"
#include "vbosphere.h"
#include <glm/glm.hpp>
#include <vector>
#include <string>

using namespace std;

class Point {
public:
    float x, y, z;
    string toString();
    glm::vec3 toVec3() const;
};

class Atom {
public:
    Point center;
    float radius;
    char  name[3];
    string toString();
};

class Molecule {
private:
    vector<VBOSphere> spheres;
    bool generateSpheres();
public:
    vector<Atom> atoms;
    Point min, max;
    string toString();
    Molecule fromPDB(char *file);
    Camera resetCamera();
    void render_vanderWalls(Shader, Camera, const int, const int, float = 0.f, float = 0.f) const;
};

int getMoleculeFromPDB(char*, Molecule&);

#endif