#include "pdbreader.hpp"
#include <stdio.h>
#include <string.h>

int getMoleculeFromPDB(char *file, Molecule &molecule) { 
    FILE *fp;
    Atom *atom;
    char line[85];
    int nm = 0;
    fp = fopen(file, "r");

    while (true) {
        fgets(line, 84, fp);
        if(!strncmp(line, "END", 3)) break;
        if(!strncmp(line, "ATOM", 4)) {
            nm++;
            atom = new Atom();
            sscanf(&line[30], "%f", &atom->center.x);
            molecule.min.x = atom->center.x < molecule.min.x ? atom->center.x : molecule.min.x;
            molecule.max.x = atom->center.x > molecule.max.x ? atom->center.x : molecule.max.x;
            sscanf(&line[38], "%f", &atom->center.y);
            molecule.min.y = atom->center.y < molecule.min.y ? atom->center.y : molecule.min.y;
            molecule.max.y = atom->center.y > molecule.max.y ? atom->center.y : molecule.max.y;
            sscanf(&line[46], "%f", &atom->center.z);
            molecule.min.z = atom->center.z < molecule.min.z ? atom->center.z : molecule.min.z;
            molecule.max.z = atom->center.z > molecule.max.z ? atom->center.z : molecule.max.z;
            sscanf(&line[77], "%s", &atom->name[0]);
            molecule.atoms.push_back(*atom);
        }
    }

    fclose(fp);
    return nm;
}

