#include "pdbreader.hpp"
#include "vbosphere.h"
#include "ptable.h"
#include <stdio.h>
#include <string.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
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
            atom.radius = PeriodicTable::getVanDerWallsRadiusFromSymbol(atom.name) / 100.0f;
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


void Molecule::render_vanderWalls(Shader shader, Camera camera, const int SCR_WIDTH, const int SCR_HEIGHT) const {
    glm::vec3 lightColor  = glm::vec3(0.0f, 0.0f, 1.0f);
    glm::vec3 objectColor = glm::vec3(1.0f, 0.5f, 0.31f);
    glm::vec3 lightPos    = camera.Position;

    shader.use();
    for (auto atom : this->atoms) {
        VBOSphere sphere = VBOSphere(atom.radius, 10, 10);
        
        shader.setVec3("objectColor", objectColor);
        shader.setVec3("lamp.lightColor", lightColor);
        shader.setVec3("lamp.lightPos", lightPos);

        // view/projection transformations
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        shader.setMat4("projection", projection);
        shader.setMat4("view", view);

        // world transformation
        glm::mat4 model = glm::mat4(1.0f);
        glm::vec3 trans = atom.center.toVec3();
        model = glm::translate(model, trans);
        model = glm::scale(model, glm::vec3(1.0f));
        shader.setMat4("model", model);

        sphere.render();
    }
}


Camera Molecule::resetCamera(void) {
    float x = (this->min.x + this->max.x) / 2;
    float y = (this->min.y + this->max.y) / 2;
    float z = this->min.z;
    return Camera(glm::vec3(x, y, z));
}


string Point::toString(void) {
    std::ostringstream repr;
    repr << "(" << this->x << ", " << this->y << ", " << this->z << ")";
    return repr.str();
}


glm::vec3 Point::toVec3(void) {
    return glm::vec3(this->x, this->y, this->z);
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

