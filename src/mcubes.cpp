#include "mcubes.h"
#include "math.h"

#include "debug.h"
#include <iostream>
#include <bitset>
#include <thread>

using namespace std;

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
    // glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glBindVertexArray(vaoHandle);
    glDrawArrays(GL_TRIANGLES, 0, vertices.size());
    // glDisable(GL_POLYGON_OFFSET_FILL);
}

// void MarchingCubes::foo(MarchingCubes _m, PiSurface _p, Point3D min, Point3D max, float maxRadius, float step, int th_curr, int th_lim, vector<GLfloat> _v, vector<GLfloat> _n) {
//     MarchingCubes &m   = const_cast<MarchingCubes&>(_m);
//     PiSurface &p       = const_cast<PiSurface&>(_p);
//     vector<GLfloat> &v = const_cast<vector<GLfloat>&>(_v);
//     vector<GLfloat> &n = const_cast<vector<GLfloat>&>(_n);
// 
//     float z0 = floor1(min.z - maxRadius);
//     float z1 = ceil1(max.z + maxRadius);
//     float zs = z1 - z0;
//     float zi = z0 + (zs / th_lim * th_curr);
//     float zf = z0 + (zs / th_lim * (th_curr + 1));
// 
//     for (float z = zi; z < zf; z += step) {
//         for (float x = floor1(min.x - maxRadius); x < ceil1(max.x + maxRadius); x += step) {
//             for (float y = floor1(min.y - maxRadius); y < ceil1(max.y + maxRadius); y += step) {
//                 m.generate_single(v, n, p, {x, y, z}, step);
//             }
//         }
//     }
// };


void MarchingCubes::generate(PiSurface p, Point3D min, Point3D max, float maxRadius) {
    this->vertices.clear();
    this->normals.clear();

    Point3D mesh_size = {
        ceil1(max.x - min.x + maxRadius),
        ceil1(max.y - min.y + maxRadius),
        ceil1(max.z - min.z + maxRadius)
    };
    float step = 0.2f;

    // debugs("\tMesh size: {%.2f, %.2f, %.2f}\n", mesh_size.x, mesh_size.y, mesh_size.z);
    // debugs("\tx = %.2lf to %.2lf step %.2f\n", floor1(min.x - maxRadius), ceil1(max.x + maxRadius), step);
    // debugs("\ty = %.2lf to %.2lf step %.2f\n", floor1(min.y - maxRadius), ceil1(max.y + maxRadius), step);
    // debugs("\tz = %.2lf to %.2lf step %.2f\n", floor1(min.z - maxRadius), ceil1(max.z + maxRadius), step);

    // auto f = [](MarchingCubes &m, PiSurface &p, Point3D min, Point3D max, float maxRadius, float step, int th_curr, int th_lim, vector<GLfloat> &v, vector<GLfloat> &n) {
    //     float z0 = floor1(min.z - maxRadius);
    //     float z1 = ceil1(max.z + maxRadius);
    //     float zs = z1 - z0;
    //     float zi = z0 + (zs / th_lim * th_curr);
    //     float zf = z0 + (zs / th_lim * (th_curr + 1));
    // 
    //     for (float z = zi; z < zf; z += step) {
    //         for (float x = floor1(min.x - maxRadius); x < ceil1(max.x + maxRadius); x += step) {
    //             for (float y = floor1(min.y - maxRadius); y < ceil1(max.y + maxRadius); y += step) {
    //                 m.generate_single(v, n, p, {x, y, z}, step);
    //             }
    //         }
    //     }
    // };

    // unsigned concurrency = thread::hardware_concurrency();
    // unsigned const c = (concurrency == 0) ? 1 : concurrency;
    // thread th[c];
    // vector<GLfloat> vert[c];
    // vector<GLfloat> norm[c];

    // for (int i = 0; i < c; i++) {
    //     th[i] = thread(&MarchingCubes::foo, this, std::ref(p), min, max, maxRadius, step, i, c, std::ref(vert[c]), std::ref(norm[c]));
    //     debugs("\tStarted thread #%d\n", i);
    // }
    // for (int i = 0; i < c; i++) {
    //     th[i].join();
    //     debugs("\tFinished thread #%d\n", i);
    // }

    // for (int i = 0; i < c; i++) {
    //     this->vertices.insert(vertices.end(), vert[c].begin(), vert[c].end());
    //     this->normals.insert(normals.end(), norm[c].begin(), norm[c].end());
    // }

    for (float z = floor1(min.z - maxRadius); z < ceil1(max.z + maxRadius); z += step) {
        for (float x = floor1(min.x - maxRadius); x < ceil1(max.x + maxRadius); x += step) {
            for (float y = floor1(min.y - maxRadius); y < ceil1(max.y + maxRadius); y += step) {
                this->generate_single(vertices, normals, p, {x, y, z}, step);
            }
        }
    }
    
    // debugs("\tGot %ld points (i.e. %ld triangles) from %lld cubes.\n", vertices.size(), vertices.size() / 3, c);

    float *v = new float[vertices.size()];
    float *n = new float[normals.size()];

    /*
    int j = 0;
    for (int i = vertices.size() - 3; i >= 0; i -= 3) {
        v[j  ] = vertices[i  ];
        v[j+1] = vertices[i+1];
        v[j+2] = vertices[i+2];
        j += 3;
    }
    j = 0;
    for (int i = normals.size() - 3; i >= 0; i -= 3) {
        n[j  ] = -normals[i  ];
        n[j+1] = -normals[i+1];
        n[j+2] = -normals[i+2];
        j += 3;
    }
    */

    unsigned int handle[2];
    glGenBuffers(2, handle);

    glBindBuffer(GL_ARRAY_BUFFER, handle[0]);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data() /*&v[0]*/, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, handle[1]);
    glBufferData(GL_ARRAY_BUFFER, normals.size()  * sizeof(GLfloat), normals.data() /*&n[0]*/, GL_STATIC_DRAW);

    glGenVertexArrays(1, &vaoHandle);
    glBindVertexArray(vaoHandle);

    glEnableVertexAttribArray(0);  // Vertex position
    glBindBuffer(GL_ARRAY_BUFFER, handle[0]);
    glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glEnableVertexAttribArray(1);  // Vertex normal
    glBindBuffer(GL_ARRAY_BUFFER, handle[1]);
    glVertexAttribPointer((GLuint)1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glBindVertexArray(0);

    delete [] v;
    delete [] n;
}


void MarchingCubes::generate_single(vector<GLfloat> &v, vector<GLfloat> &n, PiSurface surf, Point3D point, float step) {
    float w = surf.getBlendingParam();
    
    Grid grid[8];
    bool isblob = false, temp = false;
    for (int i = 0; i < 8; i++) {
        grid[i].point = {
            point.x + step * a2fVertexOffset[i][0],
            point.y + step * a2fVertexOffset[i][1],
            point.z + step * a2fVertexOffset[i][2],
        };
        grid[i].val = surf.getValueAt(grid[i].point, &temp);
        if (temp) isblob = true;
    }
    if (isblob)
        for (int i = 0; i < 8; i++)
            grid[i].val = w+1.f; //fabs(grid[i].val);

    GLint cubeIndex = 0b00000000;
    for (int i = 0; i < 8; i++) {
        if (grid[i].val <= w)
            cubeIndex |= 1 << i;
    }

    GLint edgeFlags = aiCubeEdgeFlags[cubeIndex];
    if (edgeFlags == 0) return;


    Point3D edgeVert[12];
    Vector3D edgeNorm[12];
    GLfloat fOffset; // = step / 2.f;
    for (int edge = 0; edge < 12; edge++) {
        if (edgeFlags & (1 << edge)) {
            fOffset = getOffset(grid[a2iEdgeConnection[edge][0]].val, grid[a2iEdgeConnection[edge][1]].val, w);
            edgeVert[edge].x = (point.x + (a2fVertexOffset[a2iEdgeConnection[edge][0]][0] + fOffset * a2fEdgeDirection[edge][0]) * step);
            edgeVert[edge].y = (point.y + (a2fVertexOffset[a2iEdgeConnection[edge][0]][1] + fOffset * a2fEdgeDirection[edge][1]) * step);
            edgeVert[edge].z = (point.z + (a2fVertexOffset[a2iEdgeConnection[edge][0]][2] + fOffset * a2fEdgeDirection[edge][2]) * step);
            edgeNorm[edge] = getNormal(surf, edgeVert[edge].x, edgeVert[edge].y, edgeVert[edge].z, edgeFlags);
        }
    }

    GLint ivertex;
    unsigned short wasZero;
    Point3D lastVert[3];
    Vector3D specialNorm;
    for (int t = 0; t < 5; t++) {
        wasZero = 0;
        if (a2iTriangleConnectionTable[cubeIndex][3*t] < 0)
            break;
        
        for (int c = 0; c < 3; c++) {
            ivertex = a2iTriangleConnectionTable[cubeIndex][3*t+c];
            // this->vertices.push_back(edgeVert[ivertex].x);
            // this->vertices.push_back(edgeVert[ivertex].y);
            // this->vertices.push_back(edgeVert[ivertex].z);
            v.push_back(edgeVert[ivertex].x);
            v.push_back(edgeVert[ivertex].y);
            v.push_back(edgeVert[ivertex].z);
            
            if (edgeNorm[ivertex] == (Vector3D){0.f, 0.f, 0.f}) {
                wasZero += 1;
                lastVert[c] = edgeVert[ivertex];
            }

            // this->normals.push_back(edgeNorm[ivertex].x);
            // this->normals.push_back(edgeNorm[ivertex].y);
            // this->normals.push_back(edgeNorm[ivertex].z);
            n.push_back(edgeNorm[ivertex].x);
            n.push_back(edgeNorm[ivertex].y);
            n.push_back(edgeNorm[ivertex].z);
        }

        if (wasZero == 3) {
            for (int c = 0; c < 9; c++){
                // this->normals.pop_back();
                n.pop_back();
            }
            specialNorm = getNormal(lastVert[0], lastVert[1], lastVert[2]);
            for (int c = 0; c < 3; c++) {
                // this->normals.push_back(specialNorm.x);
                // this->normals.push_back(specialNorm.y);
                // this->normals.push_back(specialNorm.z);
                n.push_back(specialNorm.x);
                n.push_back(specialNorm.y);
                n.push_back(specialNorm.z);
            }
        }
    }
}


GLfloat MarchingCubes::getOffset(GLfloat fValue1, GLfloat fValue2, GLfloat fValueDesired) {
    GLdouble fDelta = fValue2 - fValue1;
    if (fDelta == 0.0)
        return 0.5;
    return (fValueDesired - fValue1) / fDelta;
}


Vector3D MarchingCubes::getNormal(PiSurface& surf, GLfloat x, GLfloat y, GLfloat z, int edges) {
    Vector3D rfNormal;
    rfNormal.x = (surf.getValueAt(x-0.01f, y, z) - surf.getValueAt(x+0.01f, y, z));
    rfNormal.y = (surf.getValueAt(x, y-0.01f, z) - surf.getValueAt(x, y+0.01f, z));
    rfNormal.z = (surf.getValueAt(x, y, z-0.01f) - surf.getValueAt(x, y, z+0.01f));

    /* if (rfNormal.x == 0.f && rfNormal.y == 0.f && rfNormal.z == 0.f) {
        if (edges & 0b001100010001) {
            rfNormal = rfNormal + (Vector3D){0.f, -1.0f, 0.f};
        }
        if (edges & 0b011000100010) {
            rfNormal = rfNormal + (Vector3D){1.f, 0.f, 0.f};
        }
        if (edges & 0b110001000100) {
            rfNormal = rfNormal + (Vector3D){0.f, 1.f, 0.f};
        }
        if (edges & 0b100110001000) {
            rfNormal = rfNormal + (Vector3D){-1.f, 0.f, 0.f};
        }
        if (edges & 0b000011110000) {
            rfNormal = rfNormal + (Vector3D){0.f, 0.f, 1.f};
        }
        if (edges & 0b000000001111) {
            rfNormal = rfNormal + (Vector3D){0.f, 0.f, -1.f};
        }
    } */

    normalizeVector(rfNormal, rfNormal);
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


GLvoid MarchingCubes::normalizeVector(Vector3D &rfVectorResult, Vector3D &rfVectorSource) {
    GLfloat fOldLength;
    GLfloat fScale;

    fOldLength = sqrtf( (rfVectorSource.x * rfVectorSource.x) +
                        (rfVectorSource.y * rfVectorSource.y) +
                        (rfVectorSource.z * rfVectorSource.z) );

    if (fOldLength == 0.0f) {
        rfVectorResult.x = rfVectorSource.x;
        rfVectorResult.y = rfVectorSource.y;
        rfVectorResult.z = rfVectorSource.z;
    } else {
        fScale = 1.0 / fOldLength;
        rfVectorResult.x = rfVectorSource.x * fScale;
        rfVectorResult.y = rfVectorSource.y * fScale;
        rfVectorResult.z = rfVectorSource.z * fScale;
    }
}