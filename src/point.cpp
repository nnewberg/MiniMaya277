#include "point.h"

#include <iostream>

#include <la.h>


static const int pt_IDX_COUNT = 1;
static const int pt_VERT_COUNT = 1;

glm::vec4 g_point_position;

Point::Point()
    : bufIdx(QOpenGLBuffer::IndexBuffer),
      bufPos(QOpenGLBuffer::VertexBuffer),
      bufNor(QOpenGLBuffer::VertexBuffer),
      bufCol(QOpenGLBuffer::VertexBuffer)
{
    point_pos = glm::vec4(0,0,0,1);
}

void Point::setPointPos(glm::vec4 p) {
    g_point_position = p;
}

// These are functions that are only defined in this cpp file. They're used for organizational purposes
// when filling the arrays used to hold the vertex and index data.
void createPointVertexPositions(glm::vec4 (&pt_vert_pos)[pt_VERT_COUNT], glm::vec4 position)
{
    pt_vert_pos[0] = position;
}

void createPointVertexNormals(glm::vec4 (&pt_vert_nor)[pt_VERT_COUNT])
{
    pt_vert_nor[0] = glm::vec4(0,0,0,0);
}

void createPointColors(glm::vec4 (&pt_vert_col)[pt_VERT_COUNT])
{
    glm::vec4 c = glm::vec4(1,1,0,1);
    pt_vert_col[0] = c;
}


void createPointIndices(GLuint (&pt_idx)[pt_IDX_COUNT])
{
    pt_idx[0] = 0;
}

void Point::create(glm::vec4 pos)
{
    GLuint pt_idx[pt_IDX_COUNT];
    glm::vec4 pt_vert_pos[pt_VERT_COUNT];
    glm::vec4 pt_vert_nor[pt_VERT_COUNT];
    glm::vec4 pt_vert_col[pt_VERT_COUNT];

    createPointVertexPositions(pt_vert_pos, pos);
    createPointVertexNormals(pt_vert_nor);
    createPointColors(pt_vert_col);
    createPointIndices(pt_idx);

    count = pt_IDX_COUNT;

    bufIdx.create();
    bufIdx.bind();
    bufIdx.setUsagePattern(QOpenGLBuffer::StaticDraw);
    bufIdx.allocate(pt_idx, pt_IDX_COUNT * sizeof(GLuint));

    bufPos.create();
    bufPos.bind();
    bufPos.setUsagePattern(QOpenGLBuffer::StaticDraw);
    bufPos.allocate(pt_vert_pos, pt_VERT_COUNT * sizeof(glm::vec4));

    bufCol.create();
    bufCol.bind();
    bufCol.setUsagePattern(QOpenGLBuffer::StaticDraw);
    bufCol.allocate(pt_vert_col, pt_VERT_COUNT * sizeof(glm::vec4));

    bufNor.create();
    bufNor.bind();
    bufNor.setUsagePattern(QOpenGLBuffer::StaticDraw);
    bufNor.allocate(pt_vert_nor, pt_VERT_COUNT * sizeof(glm::vec4));
}

void Point::destroy()
{
    bufIdx.destroy();
    bufPos.destroy();
    bufNor.destroy();
    bufCol.destroy();
}

GLenum Point::drawMode()
{
    return GL_POINTS;
}

int Point::elemCount()
{
    return count;
}

bool Point::bindIdx()
{
    return bufIdx.bind();
}

bool Point::bindPos()
{
    return bufPos.bind();
}

bool Point::bindNor()
{
    return bufNor.bind();
}

bool Point::bindCol()
{
    return bufCol.bind();
}

bool Point::bindIds()
{
    return true;
}

bool Point::bindWts()
{
    return true;
}
