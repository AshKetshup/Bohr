#ifndef PISURF_H
#define PISURF_H

#include <vector>

using namespace std;

typedef struct {
    float x, y, z;
} Point3D;

typedef struct {
    float x, y, z;
} Vector3D;

Vector3D operator - (Point3D, Point3D);
Point3D operator - (Point3D);
Vector3D operator + (Vector3D, Vector3D);
Vector3D operator - (Vector3D);
Vector3D operator - (Vector3D, Vector3D);

class Sphere {
private:
    float r;
public:
    Point3D c;
    Sphere();
    Sphere(float, float, float, float);
    Sphere(Point3D, float);
    float getRadius(void);
    float getValueAt(float, float, float);
    float getValueAt(Point3D);
    bool pointInside(float, float, float);
    bool pointInside(Point3D);
    bool pointInCube(float, float, float);
    bool pointInCube(Point3D);
};

class PiSurface {
private:
    float w;
public:
    vector<Sphere> spheres;
    PiSurface();
    PiSurface(float);
    void reset(void);
    float getBlendingParam(void);
    void setBlendingParam(float);
    void addSphere(Sphere);
    float getValueAt(float, float, float, float = 0.f);
    float getValueAt(Point3D, float = 0.f);
    bool pointInside(float, float, float);
    bool pointInside(Point3D);
};

#endif