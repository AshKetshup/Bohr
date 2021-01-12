#include "pdbreader.hpp"
#include <stdio.h>
#include <string.h>
#include <sstream>

int getMoleculeFromPDB(char *file, Molecule &molecule) { 
    FILE *fp;
    // Atom *atom;
    char line[85];
    int na = 0;
    fp = fopen(file, "r");

    while (!feof(fp)) {
        fgets(line, 84, fp);
        if(!strncmp(line, "END", 3)) break;
        if(!strncmp(line, "ATOM", 4) || !strncmp(line, "HETATM", 6)) {
            na++;
            Atom atom = Atom();
            sscanf(&line[30], "%f", &atom.center.x);
            molecule.min.x = (na == 1) ? (atom.center.x) : (atom.center.x < molecule.min.x ? atom.center.x : molecule.min.x);
            molecule.max.x = (na == 1) ? (atom.center.x) : (atom.center.x > molecule.max.x ? atom.center.x : molecule.max.x);
            sscanf(&line[38], "%f", &atom.center.y);
            molecule.min.y = (na == 1) ? (atom.center.y) : (atom.center.y < molecule.min.y ? atom.center.y : molecule.min.y);
            molecule.max.y = (na == 1) ? (atom.center.y) : (atom.center.y > molecule.max.y ? atom.center.y : molecule.max.y);
            sscanf(&line[46], "%f", &atom.center.z);
            molecule.min.z = (na == 1) ? (atom.center.z) : (atom.center.z < molecule.min.z ? atom.center.z : molecule.min.z);
            molecule.max.z = (na == 1) ? (atom.center.z) : (atom.center.z > molecule.max.z ? atom.center.z : molecule.max.z);
            sscanf(&line[77], "%s", &atom.name[0]);
            molecule.atoms.push_back(atom);
        }
    }

    fclose(fp);
    return na;
}


Molecule Molecule::fromPDB(char *file) {
    getMoleculeFromPDB(file, *this);
    return *this;
}


string Point::toString(void) {
    std::ostringstream repr;
    repr << "(" << this->x << ", " << this->y << ", " << this->z << ")";
    return repr.str();
}


string Atom::toString(void) {
    std::ostringstream repr;
    repr << "Atom: " << this->name << "\t" << this->center.toString() << "\t" << this->radius;
    return repr.str();
}


string Molecule::toString(void) {
    std::ostringstream repr;
    for (auto atom : this->atoms) {
        repr << atom.toString() << endl;
    }
    repr << "Min: " << this->min.toString() << endl;
    repr << "Max: " << this->max.toString() << endl;
    return repr.str();
}

