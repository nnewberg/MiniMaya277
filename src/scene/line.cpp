#include "Line.h"

#include <iostream>

#include <la.h>


static const int ln_IDX_COUNT = 2;
static const int ln_VERT_COUNT = 2;


Line::Line()
    : bufIdx(QOpenGLBuffer::IndexBuffer),
      bufPos(QOpenGLBuffer::VertexBuffer),
      bufNor(QOpenGLBuffer::VertexBuffer),
      bufCol(QOpenGLBuffer::VertexBuffer)
{
}

// These are functions that are only defined in this cpp file. They're used for organizational purposes
// when filling the arrays used to hold the vertex and index data.
void createLineVertexPositions(glm::vec4 (&ln_vert_pos)[ln_VERT_COUNT], glm::vec4 line_start, glm::vec4 line_end)
{
    ln_vert_pos[0] = line_start;
    ln_vert_pos[1] = line_end;
}

void createLineVertexNormals(glm::vec4 (&ln_vert_nor)[ln_VERT_COUNT])
{
    ln_vert_nor[0] = glm::vec4(0,0,0,0);
    ln_vert_nor[1] = glm::vec4(0,0,0,0);
}

void createLineColors(glm::vec4 (&ln_vert_col)[ln_VERT_COUNT], bool isSkel)
{
    if (!isSkel) {
        glm::vec4 c = glm::vec4(1,1,1,1);
        ln_vert_col[0] = c;
        ln_vert_col[1] = c;
    }
    else {
        ln_vert_col[0] = glm::vec4(1,0,0,1);
        ln_vert_col[1] = glm::vec4(1,1,0,1);
    }
}


void createLineIndices(GLuint (&ln_idx)[ln_IDX_COUNT])
{
    ln_idx[0] = 0;
    ln_idx[1] = 1;
}

// pos0 = parent, pos1 = child
void Line::create(glm::vec4 pos0, glm::vec4 pos1, bool isSkel)
{
    GLuint ln_idx[ln_IDX_COUNT];
    glm::vec4 ln_vert_pos[ln_VERT_COUNT];
    glm::vec4 ln_vert_nor[ln_VERT_COUNT];
    glm::vec4 ln_vert_col[ln_VERT_COUNT];

    createLineVertexPositions(ln_vert_pos, pos0, pos1);
    createLineVertexNormals(ln_vert_nor);
    createLineColors(ln_vert_col, isSkel);
    createLineIndices(ln_idx);

    count = ln_IDX_COUNT;

    bufIdx.create();
    bufIdx.bind();
    bufIdx.setUsagePattern(QOpenGLBuffer::StaticDraw);
    bufIdx.allocate(ln_idx, ln_IDX_COUNT * sizeof(GLuint));

    bufPos.create();
    bufPos.bind();
    bufPos.setUsagePattern(QOpenGLBuffer::StaticDraw);
    bufPos.allocate(ln_vert_pos, ln_VERT_COUNT * sizeof(glm::vec4));

    bufCol.create();
    bufCol.bind();
    bufCol.setUsagePattern(QOpenGLBuffer::StaticDraw);
    bufCol.allocate(ln_vert_col, ln_VERT_COUNT * sizeof(glm::vec4));

    bufNor.create();
    bufNor.bind();
    bufNor.setUsagePattern(QOpenGLBuffer::StaticDraw);
    bufNor.allocate(ln_vert_nor, ln_VERT_COUNT * sizeof(glm::vec4));
}

void Line::destroy()
{
    bufIdx.destroy();
    bufPos.destroy();
    bufNor.destroy();
    bufCol.destroy();
}

GLenum Line::drawMode()
{
    return GL_LINES;
}

int Line::elemCount()
{
    return count;
}

bool Line::bindIdx()
{
    return bufIdx.bind();
}

bool Line::bindPos()
{
    return bufPos.bind();
}

bool Line::bindNor()
{
    return bufNor.bind();
}

bool Line::bindCol()
{
    return bufCol.bind();
}

bool Line::bindIds()
{
    return true;
}

bool Line::bindWts()
{
    return true;
}
