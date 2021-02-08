#include "mcubes.h"
#include "math.h"

#include "debug.h"
#include <iostream>
#include <bitset>

inline double ceil1(double x) {
    if (x >= 0.0)
        return static_cast<double>(static_cast<int>(x * 10.) + 1) / 10.;
    else
        return static_cast<double>(static_cast<int>(x * 10.) - 1) / 10.;
}

inline double floor1(double x) {
    if (x >= 0.0)
        return static_cast<double>(static_cast<int>(x * 10.) - 1) / 10.;
    else
        return static_cast<double>(static_cast<int>(x * 10.) + 1) / 10.;
}

MarchingCubes::MarchingCubes(void) {}


void MarchingCubes::render() const {
    // glEnable(GL_BLEND);
    // glBlendFunc(GL_ONE, GL_ZERO);
    // glDepthFunc(GL_GREATER);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    
    glBindVertexArray(vaoHandle);
    glDrawArrays(GL_TRIANGLES, 0, vertices.size());

    glDisable(GL_POLYGON_OFFSET_FILL);
    // glDepthFunc(GL_LESS);
    // glBindVertexArray(0);
    // glDisable(GL_BLEND);
}


void MarchingCubes::generate(PiSurface p, Point3D min, Point3D max, float maxRadius) {
    this->vertices.clear();
    this->normals.clear();

    Point3D mesh_size = {
        ceil1(max.x - min.x + maxRadius),
        ceil1(max.y - min.y + maxRadius),
        ceil1(max.z - min.z + maxRadius)
    };
    float step = 0.2f;

    debugs("\tMesh size: {%.2f, %.2f, %.2f}\n", mesh_size.x, mesh_size.y, mesh_size.z);
    debugs("\tx = %.2lf to %.2lf step %.2f\n", floor1(min.x - maxRadius), ceil1(max.x + maxRadius), step);
    debugs("\ty = %.2lf to %.2lf step %.2f\n", floor1(min.y - maxRadius), ceil1(max.y + maxRadius), step);
    debugs("\tz = %.2lf to %.2lf step %.2f\n", floor1(min.z - maxRadius), ceil1(max.z + maxRadius), step);

    unsigned long long c = 0;
    for (float z = floor1(min.z - maxRadius); z < ceil1(max.z + maxRadius); z += step) {
        for (float x = floor1(min.x - maxRadius); x < ceil1(max.x + maxRadius); x += step) {
            for (float y = floor1(min.y - maxRadius); y < ceil1(max.y + maxRadius); y += step) {
                this->generate_single(p, {x, y, z}, step);
                c++;
            }
        }
    }


    debugs("\tGot %ld points (i.e. %ld triangles) from %lld cubes.\n", vertices.size(), vertices.size() / 3, c);

    unsigned int handle[2];
    glGenBuffers(2, handle);

    glBindBuffer(GL_ARRAY_BUFFER, handle[0]);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, handle[1]);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(GLfloat), &normals[0], GL_STATIC_DRAW);

    glGenVertexArrays(1, &vaoHandle);
    glBindVertexArray(vaoHandle);

    glEnableVertexAttribArray(0);  // Vertex position
    glBindBuffer(GL_ARRAY_BUFFER, handle[0]);
    glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glEnableVertexAttribArray(1);  // Vertex normal
    glBindBuffer(GL_ARRAY_BUFFER, handle[1]);
    glVertexAttribPointer((GLuint)1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glBindVertexArray(0);
}


void MarchingCubes::generate_single(PiSurface surf, Point3D point, float step) {
    float w = surf.getBlendingParam();
    
    Grid grid[8];
    for (int i = 0; i < 8; i++) {
        grid[i].point = {
            point.x + step * a2fVertexOffset[i][0],
            point.y + step * a2fVertexOffset[i][1],
            point.z + step * a2fVertexOffset[i][2],
        };
        grid[i].val = surf.getValueAt(grid[i].point);
    }

    GLint cubeIndex = 0b00000000;
    for (int i = 0; i < 8; i++) {
        if (grid[i].val <= w)
            cubeIndex |= 1 << i;
    }

    GLint edgeFlags = aiCubeEdgeFlags[cubeIndex];
    if (edgeFlags == 0) return;


    Point3D edgeVert[12];
    Point3D edgeNorm[12];
    GLfloat fOffset; // = step / 2.f;
    for (int edge = 0; edge < 12; edge++) {
        if (edgeFlags & (1 << edge)) {
            fOffset = getOffset(grid[a2iEdgeConnection[edge][0]].val, grid[a2iEdgeConnection[edge][1]].val, w);
            edgeVert[edge].x = (point.x + (a2fVertexOffset[a2iEdgeConnection[edge][0]][0] + fOffset * a2fEdgeDirection[edge][0]) * step);
            edgeVert[edge].y = (point.y + (a2fVertexOffset[a2iEdgeConnection[edge][0]][1] + fOffset * a2fEdgeDirection[edge][1]) * step);
            edgeVert[edge].z = (point.z + (a2fVertexOffset[a2iEdgeConnection[edge][0]][2] + fOffset * a2fEdgeDirection[edge][2]) * step);
            edgeNorm[edge] = getNormal(surf, edgeVert[edge].x, edgeVert[edge].y, edgeVert[edge].z);
        }
    }

    GLint ivertex;
    for (int t = 0; t < 5; t++) {
        if (a2iTriangleConnectionTable[cubeIndex][3*t] < 0)
            break;
        
        for (int c = 0; c < 3; c++) {
            ivertex = a2iTriangleConnectionTable[cubeIndex][3*t+c];
            this->vertices.push_back(edgeVert[ivertex].x);
            this->vertices.push_back(edgeVert[ivertex].y);
            this->vertices.push_back(edgeVert[ivertex].z);
            this->normals.push_back(edgeNorm[ivertex].x);
            this->normals.push_back(edgeNorm[ivertex].y);
            this->normals.push_back(edgeNorm[ivertex].z);
        }
    }
}


GLfloat MarchingCubes::getOffset(GLfloat fValue1, GLfloat fValue2, GLfloat fValueDesired) {
    GLdouble fDelta = fValue2 - fValue1;
    if (fDelta == 0.0)
        return 0.5;
    return (fValueDesired - fValue1) / fDelta;
}


Point3D MarchingCubes::getNormal(PiSurface& surf, GLfloat x, GLfloat y, GLfloat z) {
    Point3D rfNormal;
    rfNormal.x = (surf.getValueAt(x-0.01f, y, z) - surf.getValueAt(x+0.01f, y, z));
    rfNormal.y = (surf.getValueAt(x, y-0.01f, z) - surf.getValueAt(x, y+0.01f, z));
    rfNormal.z = (surf.getValueAt(x, y, z-0.01f) - surf.getValueAt(x, y, z+0.01f));
    // normalizeVector(rfNormal, rfNormal);
    return rfNormal;
}

Vector3D MarchingCubes::getNormal(Point3D p1, Point3D p2, Point3D p3) {
    Vector3D v = p2 - p1;
    Vector3D w = p3 - p1;
    return {
        (v.y * w.z) - (v.z * w.y),
        (v.z * w.x) - (v.x * w.z),
        (v.x * w.y) - (v.y * w.x)
    };
}


GLvoid MarchingCubes::normalizeVector(Point3D &rfVectorResult, Point3D &rfVectorSource) {
    GLfloat fOldLength;
    GLfloat fScale;

    fOldLength = sqrtf( (rfVectorSource.x * rfVectorSource.x) +
                        (rfVectorSource.y * rfVectorSource.y) +
                        (rfVectorSource.z * rfVectorSource.z) );
    
    // if (rfVectorSource.x == 0.f && rfVectorSource.y == 0.f && rfVectorSource.z == 0.f) {
    //     debugs("(%6.3f, %6.3f, %6.3f)\toldLength = %f\t", rfVectorSource.x, rfVectorSource.y, rfVectorSource.z, fOldLength);
    // }

    if (fOldLength == 0.0f) {
        rfVectorResult.x = rfVectorSource.x;
        rfVectorResult.y = rfVectorSource.y;
        rfVectorResult.z = rfVectorSource.z;
        // debugs("Zero\n");
    }
    else {
        fScale = 1.0 / fOldLength;
        rfVectorResult.x = rfVectorSource.x * fScale;
        rfVectorResult.y = rfVectorSource.y * fScale;
        rfVectorResult.z = rfVectorSource.z * fScale;
    }
}