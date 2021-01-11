#ifndef CAMERA_H
#define CAMERA_H

#include <vector>
using namespace std;

class Point {
public:
    float x, y, z;
};

class Atom {
public:
    Point center;
    float radius;
    char  name[2];
    Atom();
    ~Atom();
};

class Molecule {
public:
    vector<Atom> atoms;
    Point min, max;
    Molecule();
    ~Molecule();
};

Molecule getMoleculeFromPDB(char *file, vector<Atom> &atoms);

#endif