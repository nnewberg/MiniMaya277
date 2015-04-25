#include "sphere.h"

#include <iostream>

#include <la.h>

static const int SPH_IDX_COUNT = 2280;  // 760 tris * 3
static const int SPH_VERT_COUNT = 382;

Sphere::Sphere()
    : bufIdx(QOpenGLBuffer::IndexBuffer),
      bufPos(QOpenGLBuffer::VertexBuffer),
      bufNor(QOpenGLBuffer::VertexBuffer),
      bufCol(QOpenGLBuffer::VertexBuffer)
{
}


float Sphere::intersect(ray r, glm::mat4 m){
    // ray = r0 + t*rD
    // multiply ray by inverse of transformation matrix
    glm::mat4 im = la::inverse(m);
    float radius = 0.5;
    float xc = 0;
    float yc = 0;
    float zc = 0;
    glm::vec4 r0 = im*r.ray_origin;
    glm::vec4 rD = im*r.ray_direction;
    float A = rD[0]*rD[0] + rD[1]*rD[1] + rD[2]*rD[2];
    float B = 2*(rD[0]*(r0[0]-xc) + rD[1]*(r0[1]-yc) + rD[2]*(r0[2]-zc));
    float C = (r0[0] - xc)*(r0[0] - xc) + (r0[1] - yc)*(r0[1] - yc) + (r0[2] - zc)*(r0[2] - zc) - (radius*radius);

    float discriminant = (B*B) - (4*A*C);
    if (discriminant < 0.1) {
        return -1;
    }

    float t0 = ((-B) - sqrtf(discriminant))/(2*A);
    float t1 = ((-1*B) + sqrt((B*B) - 4*A*C))/(2*A);
    if ((t0 > 0.1) && (t0 < t1)) {
        glm::vec4 Po = r0 + (t0*rD);
        glm::vec4 Pw = m*Po;
        float w_t0 = glm::length(Pw - r.ray_origin);
        return w_t0;
    }

    if (t1 > 0.1) {
        glm::vec4 Po1 = r0 + (t1*rD);
        glm::vec4 Pw1 = m*Po1;
        float w_t1 = glm::length(Pw1 - r.ray_origin);
        return w_t1;
    }
    return -1;
}




// These are functions that are only defined in this cpp file. They're used for organizational purposes
// when filling the arrays used to hold the vertex and index data.
void createSphereVertexPositions(glm::vec4 (&sph_vert_pos)[SPH_VERT_COUNT])
{
    // Create rings of vertices for the non-pole vertices
    // These will fill indices 1 - 380. Indices 0 and 381 will be filled by the two pole vertices.
    glm::vec4 v;
    // i is the Z axis rotation
    for (int i = 1; i < 20; i++) {
        // j is the Y axis rotation
        for (int j = 0; j < 20; j++) {
            v = glm::rotate(glm::mat4(1.0f), j / 20.f * TWO_PI, glm::vec3(0, 1, 0))
                * glm::rotate(glm::mat4(1.0f), -i / 18.0f * PI, glm::vec3(0, 0, 1))
                * glm::vec4(0, 0.5f, 0, 1);
            sph_vert_pos[(i - 1) * 20 + j + 1] = v;
        }
    }
    // Add the pole vertices
    sph_vert_pos[0] = glm::vec4(0, 0.5f, 0, 1);
    sph_vert_pos[381] = glm::vec4(0, -0.5f, 0, 1);  // 361 - 380 are the vertices for the bottom cap
}


void createSphereVertexNormals(glm::vec4 (&sph_vert_nor)[SPH_VERT_COUNT])
{
    // Unlike a cylinder, a sphere only needs to be one normal per vertex
    // because a sphere does not have sharp edges.
    glm::vec4 v;
    // i is the Z axis rotation
    for (int i = 1; i < 19; i++) {
        // j is the Y axis rotation
        for (int j = 0; j < 20; j++) {
            v = glm::rotate(glm::mat4(1.0f), j / 20.0f * TWO_PI, glm::vec3(0, 1, 0))
                * glm::rotate(glm::mat4(1.0f), -i / 18.0f * PI, glm::vec3(0, 0, 1))
                * glm::vec4(0, 1.0f, 0, 0);
            sph_vert_nor[(i - 1) * 20 + j + 1] = v;
        }
    }
    // Add the pole normals
    sph_vert_nor[0] = glm::vec4(0, 1.0f, 0, 0);
    sph_vert_nor[381] = glm::vec4(0, -1.0f, 0, 0);
}

void createSphereColors(glm::vec4 (&sph_vert_col)[SPH_VERT_COUNT], glm::vec4 color)
{
    for (int i = 0; i < SPH_VERT_COUNT; i++) {
        sph_vert_col[i] = color;
    }
}


void createSphereIndices(GLuint (&sph_idx)[SPH_IDX_COUNT])
{
    int index = 0;
    // Build indices for the top cap (20 tris, indices 0 - 60, up to vertex 20)
    for (int i = 0; i < 19; i++) {
        sph_idx[index] = 0;
        sph_idx[index + 1] = i + 1;
        sph_idx[index + 2] = i + 2;
        index += 3;
    }
    // Must create the last triangle separately because its indices loop
    sph_idx[57] = 0;
    sph_idx[58] = 20;
    sph_idx[59] = 1;
    index += 3;

    // Build indices for the body vertices
    // i is the Z axis rotation
    for (int i = 1; i < 19; i++) {
        // j is the Y axis rotation
        for (int j = 0; j < 20; j++) {
            sph_idx[index] = (i - 1) * 20 + j + 1;
            sph_idx[index + 1] = (i - 1) * 20 +  j + 2;
            sph_idx[index + 2] = (i - 1) * 20 +  j + 22;
            sph_idx[index + 3] = (i - 1) * 20 +  j + 1;
            sph_idx[index + 4] = (i - 1) * 20 +  j + 22;
            sph_idx[index + 5] = (i - 1) * 20 +  j + 21;
            index += 6;
        }
    }

    // Build indices for the bottom cap (20 tris, indices 2220 - 2279)
    for (int i = 0; i < 19; i++) {
        sph_idx[index] = 381;
        sph_idx[index + 1] = i + 361;
        sph_idx[index + 2] = i + 362;
        index += 3;
    }
    // Must create the last triangle separately because its indices loop
    sph_idx[2277] = 381;
    sph_idx[2278] = 380;
    sph_idx[2279] = 361;
    index += 3;
}

void Sphere::create()
{
    GLuint sph_idx[SPH_IDX_COUNT];
    glm::vec4 sph_vert_pos[SPH_VERT_COUNT];
    glm::vec4 sph_vert_nor[SPH_VERT_COUNT];
    glm::vec4 sph_vert_col[SPH_VERT_COUNT];

    createSphereVertexPositions(sph_vert_pos);
    createSphereVertexNormals(sph_vert_nor);
    createSphereColors(sph_vert_col, glm::vec4(1,1,1,1));
    createSphereIndices(sph_idx);

    count = SPH_IDX_COUNT;

    bufIdx.create();
    bufIdx.bind();
    bufIdx.setUsagePattern(QOpenGLBuffer::StaticDraw);
    bufIdx.allocate(sph_idx, SPH_IDX_COUNT * sizeof(GLuint));

    bufPos.create();
    bufPos.bind();
    bufPos.setUsagePattern(QOpenGLBuffer::StaticDraw);
    bufPos.allocate(sph_vert_pos, SPH_VERT_COUNT * sizeof(glm::vec4));

    bufCol.create();
    bufCol.bind();
    bufCol.setUsagePattern(QOpenGLBuffer::StaticDraw);
    bufCol.allocate(sph_vert_col, SPH_VERT_COUNT * sizeof(glm::vec4));

    bufNor.create();
    bufNor.bind();
    bufNor.setUsagePattern(QOpenGLBuffer::StaticDraw);
    bufNor.allocate(sph_vert_nor, SPH_VERT_COUNT * sizeof(glm::vec4));
}

void Sphere::setColor(glm::vec4 color) {
    glm::vec4 sph_vert_col[SPH_VERT_COUNT];

    createSphereColors(sph_vert_col, color);
    bufCol.create();
    bufCol.bind();
    bufCol.setUsagePattern(QOpenGLBuffer::StaticDraw);
    bufCol.allocate(sph_vert_col, SPH_VERT_COUNT * sizeof(glm::vec4));

}

void Sphere::destroy()
{
    bufIdx.destroy();
    bufPos.destroy();
    bufNor.destroy();
    bufCol.destroy();
}

GLenum Sphere::drawMode()
{
    return GL_TRIANGLES;
}

int Sphere::elemCount()
{
    return count;
}

bool Sphere::bindIdx()
{
    return bufIdx.bind();
}

bool Sphere::bindPos()
{
    return bufPos.bind();
}

bool Sphere::bindNor()
{
    return bufNor.bind();
}

bool Sphere::bindCol()
{
    return bufCol.bind();
}

bool Sphere::bindIds()
{
    return true;
}

bool Sphere::bindWts()
{
    return true;
}
