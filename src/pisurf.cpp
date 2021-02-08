#include "pisurf.h"
#include "math.h"

#include "debug.h"

inline float sqr(float n) { return n * n; }

Vector3D operator - (Point3D p1, Point3D p2) {
    return {p2.x - p1.x, p2.y - p1.y, p2.z - p1.z};
}

Point3D operator - (Point3D p) {
    return {-p.x, -p.y, -p.z};
}

bool operator == (Vector3D p1, Vector3D p2) {
    return (p1.x == p2.x) && (p1.y == p2.y) && (p1.z == p2.z);
}

Vector3D operator + (Vector3D v, Vector3D w) {
    return {v.x + w.x, v.y + w.y, v.z + w.z};
}

Vector3D operator - (Vector3D v) {
    return {-v.x, -v.y, -v.z};
}

Vector3D operator - (Vector3D v, Vector3D w) {
    return v + (-w);
}

Sphere::Sphere(void) : c({0.f, 0.f, 0.f}), r(0.f) {}
Sphere::Sphere(float cx, float cy, float cz, float cr) : c({cx, cy, cz}), r(cr) {}
Sphere::Sphere(Point3D cc, float cr) : c(cc), r(cr) {}

float Sphere::getRadius(void) {
    return this->r;
}

float Sphere::getValueAt(float x, float y, float z) {
    return this->getValueAt({x, y, z});
}

float Sphere::getValueAt(Point3D p) {
    return sqr(p.x - this->c.x) + sqr(p.y - this->c.y) + sqr(p.z - this->c.z); // - sqr(this->r);
}

bool Sphere::pointInside(float x, float y, float z) {
    return this->pointInside({x, y, z});
}

bool Sphere::pointInside(Point3D p) {
    return this->getValueAt(p) <= sqr(this->r);
}

bool Sphere::pointInCube(float x, float y, float z) {
    return this->pointInCube({x, y, z});
}

bool Sphere::pointInCube(Point3D p) {
    float r = this->r;
    // float d = sqrtf(sqr(r) + sqrtf(2 * sqr(r)));
    float x0 = this->c.x - r;
    float y0 = this->c.y - r;
    float z0 = this->c.z - r;
    return (
        (x0 <= p.x) && (p.x <= x0 + 2 * r) &&
        (y0 <= p.y) && (p.y <= y0 + 2 * r) &&
        (z0 <= p.z) && (p.z <= z0 + 2 * r)
    );
}


PiSurface::PiSurface(void) : w(0.f) { spheres = vector<Sphere>(); }
PiSurface::PiSurface(float cw) : w(cw) { spheres = vector<Sphere>(); }

void PiSurface::reset(void) {
    this->spheres.clear();
    this->w = 0.f;
}

float PiSurface::getBlendingParam(void) {
    return this->w;
}

void PiSurface::setBlendingParam(float cw) {
    this->w = cw;
}

void PiSurface::addSphere(Sphere s) {
    this->spheres.push_back(s);
}

float PiSurface::getValueAt(float x, float y, float z, float padding) {
    return this->getValueAt({x, y, z}, padding);
}

float PiSurface::getValueAt(Point3D p, float padding) {
    float res = 1.f;
    for (auto s : this->spheres) {
        if (s.pointInCube(p)) {
            res *= s.getValueAt(p) - sqr(s.getRadius());
        }
    }
    return res; // - this->w;
}

bool PiSurface::pointInside(float x, float y, float z) {
    return this->pointInside({x, y, z});
}

bool PiSurface::pointInside(Point3D p) {
    return this->getValueAt(p) <= this->w;
}
