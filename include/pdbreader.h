#ifndef PDBREADER_H
#define PDBREADER_H

#include "shader_m.h"
#include "camera.h"
#include "vbosphere.h"
#include "mcubes.h"
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
    PiSurface pisurf;
    MarchingCubes mcubes;
    bool generateSpheres();
    void generatePiSurface(float);
public:
    vector<Atom> atoms;
    Point min, max;
    float maxRadius;
    string toString();
    Molecule fromPDB(const char *file);
    Camera resetCamera();
    void setBlendingParam(float);
    void render_vanderWalls(Shader, Camera, const int, const int, float = 0.f, float = 0.f) const;
    void render_piSurface(Shader, Camera, const int, const int, float = 0.f, float = 0.f) const;
};

int getMoleculeFromPDB(const char*, Molecule&);

#endif