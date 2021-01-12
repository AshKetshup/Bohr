#ifndef PDBREADER_H
#define PDBREADER_H

#include <vector>
#include <string>
using namespace std;

class Point {
public:
    float x, y, z;
    string toString();
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
};

int getMoleculeFromPDB(char *file, Molecule &molecule);

#endif