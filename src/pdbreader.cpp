#include "pdbreader.hpp"
#include "ptable.h"
#include <stdio.h>
#include <string.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <sstream>

int getMoleculeFromPDB(char *file, Molecule &molecule) { 
    FILE *fp;
    Atom atom;
    char line[85];
    int na = 0;
    fp = fopen(file, "r");

    while (!feof(fp)) {
        fgets(line, 84, fp);
        if(!strncmp(line, "END", 3)) break;
        if(!strncmp(line, "ATOM", 4) || !strncmp(line, "HETATM", 6)) {
            na++;
            atom = Atom();
            sscanf(&line[30], "%f", &atom.center.x);
            molecule.min.x = (na == 1) ? (atom.center.x) : (atom.center.x < molecule.min.x ? atom.center.x : molecule.min.x);
            molecule.max.x = (na == 1) ? (atom.center.x) : (atom.center.x > molecule.max.x ? atom.center.x : molecule.max.x);
            sscanf(&line[38], "%f", &atom.center.y);
            molecule.min.y = (na == 1) ? (atom.center.y) : (atom.center.y < molecule.min.y ? atom.center.y : molecule.min.y);
            molecule.max.y = (na == 1) ? (atom.center.y) : (atom.center.y > molecule.max.y ? atom.center.y : molecule.max.y);
            sscanf(&line[46], "%f", &atom.center.z);
            molecule.min.z = (na == 1) ? (atom.center.z) : (atom.center.z < molecule.min.z ? atom.center.z : molecule.min.z);
            molecule.max.z = (na == 1) ? (atom.center.z) : (atom.center.z > molecule.max.z ? atom.center.z : molecule.max.z);
            sscanf(&line[76], "%s", &atom.name[0]);
            atom.radius = PeriodicTable::getVanDerWallsRadiusFromSymbol(atom.name) / 100.0f;
            molecule.atoms.push_back(atom);
        }
    }

    fclose(fp);
    return na;
}


Molecule Molecule::fromPDB(char *file) {
    this->atoms.clear();
    this->spheres.clear();
    getMoleculeFromPDB(file, *this);
    this->generateSpheres();
    return *this;
}


bool Molecule::generateSpheres(void) {
    this->spheres = vector<VBOSphere>();
    for (auto atom : this->atoms) {
        this->spheres.push_back(VBOSphere(atom.radius, 50, 50, PeriodicTable::getColorFromSymbol(atom.name).toVec3()));
    }
    return true;
}


void Molecule::render_vanderWalls(Shader shader, Camera camera, const int SCR_WIDTH, const int SCR_HEIGHT, float roty, float rotz) const {
    glm::vec3 lightColor  = glm::vec3(1.0f, 1.0f, 1.0f);
    glm::vec3 lightPos    = camera.Position;
    // glm::vec3 molPos      = glm::vec3((this->min.x + this->max.x) / 2.f, (this->min.y + this->max.y) / 2.f, (this->min.z + this->max.z) / 2.f);

    shader.use();

    for (size_t i = 0; i < this->spheres.size(); i++) {
        shader.setVec3("objectColor",     this->spheres[i].color);
        shader.setVec3("lamp.lightColor", lightColor);
        shader.setVec3("lamp.lightPos",   lightPos);
        
        // view/projection transformations
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        // glm::mat4 view = glm::lookAt(camera.Position, molPos, glm::vec3(0.f, 1.f, 0.f));
        shader.setMat4("projection", projection);
        shader.setMat4("view", view);

        // world transformation
        glm::mat4 model = glm::mat4(1.0f);
        glm::vec3 trans = this->atoms[i].center.toVec3();
        model = glm::rotate(model, glm::radians(roty), glm::vec3(1.f, 0.f, 0.f));
        model = glm::rotate(model, glm::radians(rotz), glm::vec3(0.f, 1.f, 0.f));
        model = glm::translate(model, trans);
        model = glm::scale(model, glm::vec3(1.0f));
        shader.setMat4("model", model);

        this->spheres[i].render();
    }
}


Camera Molecule::resetCamera(void) {
    float x = (this->min.x + this->max.x) / 2.f;
    float y = (this->min.y + this->max.y) / 2.f;
    float z = fabs(x) + fabs(y); // this->max.z;
    Camera camera = Camera(glm::vec3(x, y, z));
    camera.MovementSpeed *= 4.f;
    return camera;
}


string Point::toString(void) {
    std::ostringstream repr;
    repr << "(" << this->x << ", " << this->y << ", " << this->z << ")";
    return repr.str();
}


glm::vec3 Point::toVec3(void) const {
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

