#include "pisurf.h"

inline float sqr(float n) { return n * n; }

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
    return sqr(p.x - this->c.x) + sqr(p.y - this->c.y) + sqr(p.z - this->c.z) - sqr(this->r);
}

bool Sphere::pointInside(float x, float y, float z) {
    return this->pointInside({x, y, z});
}

bool Sphere::pointInside(Point3D p) {
    return this->getValueAt(p) <= this->r;
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

float PiSurface::getValueAt(float x, float y, float z) {
    return this->getValueAt({x, y, z});
}

float PiSurface::getValueAt(Point3D p) {
    float res = 1.f;
    for (auto s : this->spheres) {
        res *= s.getValueAt(p);
    }
    return res - this->w;
}

bool PiSurface::pointInside(float x, float y, float z) {
    return this->pointInside({x, y, z});
}

bool PiSurface::pointInside(Point3D p) {
    return this->getValueAt(p) <= this->w;
}
