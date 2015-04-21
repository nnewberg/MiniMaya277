#include "WireSphere.h"

#include <iostream>

#include <la.h>

static const int SPH_IDX_COUNT = 72;
static const int SPH_VERT_COUNT = 36;


glm::vec4 WireSphere::getColor() const
{
    return color;
}

void WireSphere::setColor(const glm::vec4 &value)
{
    color = value;
}
WireSphere::WireSphere()
    : bufIdx(QOpenGLBuffer::IndexBuffer),
      bufPos(QOpenGLBuffer::VertexBuffer),
      bufNor(QOpenGLBuffer::VertexBuffer),
      bufCol(QOpenGLBuffer::VertexBuffer)
{
}

// These are functions that are only defined in this cpp file. They're used for organizational purposes
// when filling the arrays used to hold the vertex and index data.
void createWireSphereVertexPositions(glm::vec4 (&sph_vert_pos)[SPH_VERT_COUNT])
{
    glm::vec4 v;
    // about y-axis
    for (int i = 0; i < 12; i++) {
        v = glm::rotate(glm::mat4(1.0f), i / 12.f * TWO_PI, glm::vec3(1, 0, 0)) * glm::vec4(0, 0.5f, 0, 1);
        sph_vert_pos[i] = v;
    }
    // about x-axis
    for (int i = 12; i < 24; i++) {
        v = glm::rotate(glm::mat4(1.0f), i / 12.f * TWO_PI, glm::vec3(0, 1, 0)) * glm::vec4(0.5f, 0, 0, 1);
        sph_vert_pos[i] = v;
    }
    // about z-axis
    for (int i = 24; i < 36; i++) {
        v = glm::rotate(glm::mat4(1.0f), i / 12.f * TWO_PI, glm::vec3(0, 0, 1)) * glm::vec4(0, 0.5f, 0, 1);
        sph_vert_pos[i] = v;
    }

}


void createWireSphereVertexNormals(glm::vec4 (&sph_vert_nor)[SPH_VERT_COUNT])
{
    glm::vec4 c = glm::vec4(0,0,0,0);
    for (int i = 0; i < SPH_VERT_COUNT; i++) {
        sph_vert_nor[i] = c;
    }
}

void createWireSphereColors(glm::vec4 (&sph_vert_col)[SPH_VERT_COUNT], glm::vec4 color)
{
    for (int i = 0; i < SPH_VERT_COUNT; i++) {
        sph_vert_col[i] = color;
    }
}


void createWireSphereIndices(GLuint (&sph_idx)[SPH_IDX_COUNT])
{
    int index = 1;
    // Build indices for the top cap (20 tris, indices 0 - 60, up to vertex 20)
    sph_idx[0] = 0;
    for (int i = 1; i < 12; i++) {
        sph_idx[index] = i;
        sph_idx[index + 1] = i;
        index += 2;
    }
    sph_idx[index] = 0;
    index++;
    sph_idx[index] = 12;
    index++;
    for (int i = 13; i < 24; i++) {
        sph_idx[index] = i;
        sph_idx[index + 1] = i;
        index += 2;
    }
    sph_idx[index] = 12;

    index++;
    sph_idx[index] = 24;
    index++;
    for (int i = 25; i < 36; i++) {
        sph_idx[index] = i;
        sph_idx[index + 1] = i;
        index += 2;
    }
    sph_idx[index] = 24;

}

void WireSphere::create()
{
    GLuint sph_idx[SPH_IDX_COUNT];
    glm::vec4 sph_vert_pos[SPH_VERT_COUNT];
    glm::vec4 sph_vert_nor[SPH_VERT_COUNT];
    glm::vec4 sph_vert_col[SPH_VERT_COUNT];

    createWireSphereVertexPositions(sph_vert_pos);
    createWireSphereVertexNormals(sph_vert_nor);
    createWireSphereColors(sph_vert_col, color);
    createWireSphereIndices(sph_idx);

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

void WireSphere::update() {
    GLuint sph_idx[SPH_IDX_COUNT];
    glm::vec4 sph_vert_pos[SPH_VERT_COUNT];
    glm::vec4 sph_vert_nor[SPH_VERT_COUNT];
    glm::vec4 sph_vert_col[SPH_VERT_COUNT];

    createWireSphereVertexPositions(sph_vert_pos);
    createWireSphereVertexNormals(sph_vert_nor);
    createWireSphereColors(sph_vert_col, color);
    createWireSphereIndices(sph_idx);

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

void WireSphere::destroy()
{
    bufIdx.destroy();
    bufPos.destroy();
    bufNor.destroy();
    bufCol.destroy();
}

GLenum WireSphere::drawMode()
{
    return GL_LINES;
}

int WireSphere::elemCount()
{
    return count;
}

bool WireSphere::bindIdx()
{
    return bufIdx.bind();
}

bool WireSphere::bindPos()
{
    return bufPos.bind();
}

bool WireSphere::bindNor()
{
    return bufNor.bind();
}

bool WireSphere::bindCol()
{
    return bufCol.bind();
}

bool WireSphere::bindIds()
{
    return true;
}

bool WireSphere::bindWts()
{
    return true;
}
