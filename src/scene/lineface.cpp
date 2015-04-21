#include "LineFace.h"


#include <iostream>

#include <la.h>

int idx_count;
int vert_count;
//static const int ln_IDX_COUNT = 2;
//static const int lineface_vert_COUNT = 2;


LineFace::LineFace()
    : bufIdx(QOpenGLBuffer::IndexBuffer),
      bufPos(QOpenGLBuffer::VertexBuffer),
      bufNor(QOpenGLBuffer::VertexBuffer),
      bufCol(QOpenGLBuffer::VertexBuffer)
{
}

// These are functions that are only defined in this cpp file. They're used for organizational purposes
// when filling the arrays used to hold the vertex and index data.
void createLineFaceVertexPositions(std::vector<glm::vec4> *lineface_vert_pos, Face* f)
{
    std::vector<Vertex*> verts = f->getVertices();
    lineface_vert_pos->push_back(verts.at(0)->getPos());
    for (unsigned long i = 1; i < verts.size(); i++) {
        lineface_vert_pos->push_back(verts.at(i)->getPos());
        lineface_vert_pos->push_back(verts.at(i)->getPos());
    }
    lineface_vert_pos->push_back(verts.at(0)->getPos());
}

void createLineFaceVertexNormals(std::vector<glm::vec4> *lineface_vert_nor)
{
    glm::vec4 c = glm::vec4(0,0,0,0);
    for (int i = 0; i < idx_count; i++) {
        lineface_vert_nor->push_back(c);
    }
}

void createLineFaceColors(std::vector<glm::vec4> *lineface_vert_col)
{
    glm::vec4 c = glm::vec4(1,1,1,1);
    for (int i = 0; i < idx_count; i++) {
        lineface_vert_col->push_back(c);
    }

}


void createLineFaceIndices(std::vector<GLuint> *lineface_idx)
{
    for (int i = 0; i < idx_count; i++) {
        lineface_idx->push_back(i);
    }
}

void LineFace::create(Face* f)
{

    vert_count = f->getVertices().size();
    idx_count = vert_count*2;

    std::vector<GLuint> lineface_idx = {};
    std::vector<glm::vec4> lineface_vert_col = {};
    std::vector<glm::vec4> lineface_vert_nor = {};
    std::vector<glm::vec4> lineface_vert_pos = {};


    createLineFaceVertexPositions(&lineface_vert_pos, f);
    createLineFaceVertexNormals(&lineface_vert_nor);
    createLineFaceColors(&lineface_vert_col);
    createLineFaceIndices(&lineface_idx);

    count = idx_count;

    bufIdx.create();
    bufIdx.bind();
    bufIdx.setUsagePattern(QOpenGLBuffer::StaticDraw);
    bufIdx.allocate(lineface_idx.data(), lineface_idx.size() * sizeof(GLuint));

    bufPos.create();
    bufPos.bind();
    bufPos.setUsagePattern(QOpenGLBuffer::StaticDraw);
    bufPos.allocate(lineface_vert_pos.data(), lineface_vert_pos.size() * sizeof(glm::vec4));

    bufCol.create();
    bufCol.bind();
    bufCol.setUsagePattern(QOpenGLBuffer::StaticDraw);
    bufPos.allocate(lineface_vert_col.data(), lineface_vert_col.size() * sizeof(glm::vec4));

    bufNor.create();
    bufNor.bind();
    bufNor.setUsagePattern(QOpenGLBuffer::StaticDraw);
    bufPos.allocate(lineface_vert_nor.data(), lineface_vert_nor.size() * sizeof(glm::vec4));
}

void LineFace::destroy()
{
    bufIdx.destroy();
    bufPos.destroy();
    bufNor.destroy();
    bufCol.destroy();
}

GLenum LineFace::drawMode()
{
    return GL_LINES;
}

int LineFace::elemCount()
{
    return count;
}

bool LineFace::bindIdx()
{
    return bufIdx.bind();
}

bool LineFace::bindPos()
{
    return bufPos.bind();
}

bool LineFace::bindNor()
{
    return bufNor.bind();
}

bool LineFace::bindCol()
{
    return bufCol.bind();
}

bool LineFace::bindIds()
{
    return true;
}

bool LineFace::bindWts()
{
    return true;
}
