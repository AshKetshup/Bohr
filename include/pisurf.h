#ifndef PISURF_H
#define PISURF_H

#include <vector>

using namespace std;

typedef struct {
    float x, y, z;
} Point3D;

class Sphere {
private:
    Point3D c;
    float r;
public:
    Sphere();
    Sphere(float, float, float, float);
    Sphere(Point3D, float);
    float getRadius(void);
    float getValueAt(float, float, float);
    float getValueAt(Point3D);
    bool pointInside(float, float, float);
    bool pointInside(Point3D);
};

class PiSurface {
private:
    vector<Sphere> spheres;
    float w;
public:
    PiSurface();
    PiSurface(float);
    void reset(void);
    float getBlendingParam(void);
    void setBlendingParam(float);
    void addSphere(Sphere);
    float getValueAt(float, float, float);
    float getValueAt(Point3D);
    bool pointInside(float, float, float);
    bool pointInside(Point3D);
};

#endif