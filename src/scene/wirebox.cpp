#include "wirebox.h"
#include<la.h>
#include <iostream>
#include <math.h>

//static const int WIR_IDX_COUNT = 48;
//static const int WIR_VERT_COUNT = 24;
static const int WIR_IDX_COUNT = 24;
static const int WIR_VERT_COUNT = 8;

wirebox::wirebox()
  : bufIdx(QOpenGLBuffer::IndexBuffer),
    bufPos(QOpenGLBuffer::VertexBuffer),
    bufNor(QOpenGLBuffer::VertexBuffer),
    bufCol(QOpenGLBuffer::VertexBuffer) {
}

float wirebox::intersect(ray r, glm::mat4 m) {

    glm::mat4 im = la::inverse(m);
    glm::vec4 r0 = im*r.ray_origin;
    glm::vec4 rD = im*r.ray_direction;

    float tnear = -INFINITY;
    float tfar = INFINITY;
    glm::vec4 min = glm::vec4(-0.5, -0.5,  -0.5, 1);
    glm::vec4 max = glm::vec4(0.5, 0.5, 0.5, 1);

    if (rD[0] == 0) {
        if ((r0[0] < min[0]) || (r0[0] > max[0])) {
            return -1;
        }
    }
    float t0 = (min[0] - r0[0])/rD[0];
    float t1 = (max[0] - r0[0])/rD[0];
    if (t0 > t1) {
        float temp = t0;
        t0 = t1;
        t1 = temp;
    }
    if (t0 > tnear) {
        tnear = t0;
    }
    if (t1 < tfar) {
        tfar = t1;
    }

    if (rD[1] == 0) {
        if ((r0[1] < min[1]) || (r0[1] > max[1])) {
            return -1;
        }
    }
    t0 = (min[1] - r0[1])/rD[1];
    t1 = (max[1] - r0[1])/rD[1];
    if (t0 > t1) {
        float temp1 = t0;
        t0 = t1;
        t1 = temp1;
    }
    if (t0 > tnear) {
        tnear = t0;
    }
    if (t1 < tfar) {
        tfar = t1;
    }

    if (rD[2] == 0) {
        if ((r0[2] < min[2]) || (r0[2] > max[2])) {
            return -1;
        }
    }
    t0 = (min[2] - r0[2])/rD[2];
    t1 = (max[2] - r0[2])/rD[2];
    if (t0 > t1) {
        float temp2 = t0;
        t0 = t1;
        t1 = temp2;
    }
    if (t0 > tnear) {
        tnear = t0;
    }
    if (t1 < tfar) {
        tfar = t1;
    }
    if (tnear < tfar) {
        glm::vec4 Po = r0 + (tnear*rD);
        glm::vec4 Pw = m*Po;
        float w_tnear = glm::length(Pw - r.ray_origin);
        return w_tnear;
    }
    return -1;
}


// These are functions that are only defined in this cpp file. They're used for organizational purposes
// when filling the arrays used to hold the vertex and index data.
void createWireboxVertexPositions(glm::vec4 (&wir_vert_pos)[WIR_VERT_COUNT], std::vector<Vertex*> boxVertices) {

    for (unsigned long i = 0; i < boxVertices.size(); i++) {
        wir_vert_pos[i] = boxVertices.at(i)->getPos();
    }

}




void createWireboxIndices(GLuint (&wir_idx)[WIR_IDX_COUNT]){
  //Build indices for the top cap (indices, verts 0-3)
  for(int i = 0; i < 3; i++){
    wir_idx[2*i] = i;
    wir_idx[2*i + 1] = i+1;
  }
  wir_idx[6] = 3;
  wir_idx[7] = 0;


  //Build indices for the bottom cap (indices, verts 4-7)
  for(int i = 4; i < 7; i++){
    wir_idx[2*i] = i;
    wir_idx[2*i + 1] = i+1;
  }
  wir_idx[14] = 7;
  wir_idx[15] = 4;

  for(int i = 0; i < 4; i++) {
      wir_idx[16 + i*2] = i;
      wir_idx[17 + i*2] = i + 4;
  }
}

void createWireboxVertexColors(glm::vec4 (&wir_vert_col)[WIR_VERT_COUNT]){
    for (int i = 0; i < WIR_VERT_COUNT; i++) {
        wir_vert_col[i] = glm::vec4(1,1,0,1);
    }
}

void wirebox::create()
{
  GLuint wir_idx[WIR_IDX_COUNT];
  glm::vec4 wir_vert_pos[WIR_VERT_COUNT];
  glm::vec4 wir_vert_nor[WIR_VERT_COUNT];
  glm::vec4 wir_vert_col[WIR_VERT_COUNT];

  // Store top face verts (IDX 0 - 3)
  for(int i = 0; i < 4; i++) {

    glm::vec4 pos = glm::rotate(glm::mat4(1.0f), i / 4.f * TWO_PI, glm::vec3(0, 1, 0)) * glm::vec4(0.5f, 0.5f, 0.5f, 1);
    Vertex* v = new Vertex();
    v->setPos(pos);
    v->setId(i);
    boxVertices.push_back(v);
  }

  // Store bottom cap verts (IDX 4-7)
  for(int i = 4; i < 8; i++) {
    glm::vec4 pos = glm::rotate(glm::mat4(1.0f), i / 4.f * TWO_PI, glm::vec3(0, 1, 0)) * glm::vec4(0.5f, -0.5f, 0.5f, 1);
    Vertex* v = new Vertex();
    v->setPos(pos);
    v->setId(i);
    boxVertices.push_back(v);
  }


  createWireboxVertexPositions(wir_vert_pos, boxVertices);
//  createWireboxVertexNormals(wir_vert_nor);
  createWireboxVertexColors(wir_vert_col);
  createWireboxIndices(wir_idx);

  count = WIR_IDX_COUNT;

  bufIdx.create();
  bufIdx.bind();
  bufIdx.setUsagePattern(QOpenGLBuffer::StaticDraw);
  bufIdx.allocate(wir_idx, WIR_IDX_COUNT * sizeof(GLuint));

  bufPos.create();
  bufPos.bind();
  bufPos.setUsagePattern(QOpenGLBuffer::StaticDraw);
  bufPos.allocate(wir_vert_pos,WIR_VERT_COUNT * sizeof(glm::vec4));

  bufCol.create();
  bufCol.bind();
  bufCol.setUsagePattern(QOpenGLBuffer::StaticDraw);
  bufCol.allocate(wir_vert_col, WIR_VERT_COUNT * sizeof(glm::vec4));

  bufNor.create();
  bufNor.bind();
  bufNor.setUsagePattern(QOpenGLBuffer::StaticDraw);
  bufNor.allocate(0, 0 * sizeof(glm::vec4));
}

void wirebox::update() {
    GLuint wir_idx[WIR_IDX_COUNT];
    glm::vec4 wir_vert_pos[WIR_VERT_COUNT];
    glm::vec4 wir_vert_nor[WIR_VERT_COUNT];
    glm::vec4 wir_vert_col[WIR_VERT_COUNT];

    createWireboxVertexPositions(wir_vert_pos, boxVertices);
  //  createWireboxVertexNormals(wir_vert_nor);
    createWireboxVertexColors(wir_vert_col);
    createWireboxIndices(wir_idx);

    count = WIR_IDX_COUNT;

    bufIdx.create();
    bufIdx.bind();
    bufIdx.setUsagePattern(QOpenGLBuffer::StaticDraw);
    bufIdx.allocate(wir_idx, WIR_IDX_COUNT * sizeof(GLuint));

    bufPos.create();
    bufPos.bind();
    bufPos.setUsagePattern(QOpenGLBuffer::StaticDraw);
    bufPos.allocate(wir_vert_pos,WIR_VERT_COUNT * sizeof(glm::vec4));

    bufCol.create();
    bufCol.bind();
    bufCol.setUsagePattern(QOpenGLBuffer::StaticDraw);
    bufCol.allocate(wir_vert_col, WIR_VERT_COUNT * sizeof(glm::vec4));

    bufNor.create();
    bufNor.bind();
    bufNor.setUsagePattern(QOpenGLBuffer::StaticDraw);
    bufNor.allocate(0, 0 * sizeof(glm::vec4));
}

void wirebox::destroy()
{
  bufIdx.destroy();
  bufPos.destroy();
  bufNor.destroy();
  bufCol.destroy();
}

GLenum wirebox::drawMode()
{
  return GL_LINES;
}

int wirebox::elemCount()
{
  return count;
}

bool wirebox::bindIdx()
{
  return bufIdx.bind();
}

bool wirebox::bindPos()
{
  return bufPos.bind();
}

bool wirebox::bindNor()
{
  return bufNor.bind();
}

bool wirebox::bindCol()
{
  return bufCol.bind();
}

bool wirebox::bindIds()
{
    return true;
}

bool wirebox::bindWts()
{
    return true;
}
