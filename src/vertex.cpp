#include "vertex.h"

static const int vt_IDX_COUNT = 1;
static const int vt_VERT_COUNT = 1;
static const int vt_JOINT_COUNT = 2;
static const int vt_INFL_COUNT = 2;



Vertex::Vertex()
    : bufIdx(QOpenGLBuffer::IndexBuffer),
      bufPos(QOpenGLBuffer::VertexBuffer),
      bufNor(QOpenGLBuffer::VertexBuffer),
      bufCol(QOpenGLBuffer::VertexBuffer),
      bufJointId(QOpenGLBuffer::IndexBuffer),
      bufJointWeight(QOpenGLBuffer::IndexBuffer)
{
    pos = glm::vec4(0);
    edge = NULL;
    id = 0;
    wasSmoothed = false;
    isSharp = false;
    influenceJoints = {};
}

Vertex::Vertex(glm::vec4 p) {
    pos = p;
    edge = NULL;
    id = 0;
    wasSmoothed = false;
    isSharp = false;
}

glm::vec4 Vertex::getPos() {
    return this->pos;
}


//need to make QOpenGLBuffer for all four shader attributes
//create
// shaderprogram - link up memory

void createVertexVertexPositions(glm::vec4 (&vt_vert_pos)[vt_VERT_COUNT], glm::vec4 position)
{
    vt_vert_pos[0] = position;
}

void createVertexVertexNormals(glm::vec4 (&vt_vert_nor)[vt_VERT_COUNT])
{
    vt_vert_nor[0] = glm::vec4(0,0,0,0);
}

void createVertexColors(glm::vec4 (&vt_vert_col)[vt_VERT_COUNT])
{
    glm::vec4 c = glm::vec4(1,1,0,1);
    vt_vert_col[0] = c;
}


void createVertexIndices(GLuint (&vt_idx)[vt_IDX_COUNT])
{
    vt_idx[0] = 0;
}


//void createVertexJointIds(GLuint (&vt_jt_id)[vt_INFL_COUNT], glm::vec2 joints) {
//    vt_jt_id[0] = joints[0];
//    vt_jt_id[1] = joints[1];
//}

//void createVertexJointWeights(GLuint (&vt_jt_wt)[vt_INFL_COUNT], glm::vec2 weights) {
//    vt_jt_wt[0] = weights[0];
//    vt_jt_wt[1] = weights[1];
//}

void Vertex::create()
{
    GLuint vt_idx[vt_IDX_COUNT];
    glm::vec4 vt_vert_pos[vt_VERT_COUNT];
    glm::vec4 vt_vert_nor[vt_VERT_COUNT];
    glm::vec4 vt_vert_col[vt_VERT_COUNT];

    GLuint vt_jt_id[vt_INFL_COUNT];
    GLuint vt_jt_wt[vt_INFL_COUNT];

    createVertexVertexPositions(vt_vert_pos, pos);
    createVertexVertexNormals(vt_vert_nor);
    createVertexColors(vt_vert_col);
    createVertexIndices(vt_idx);

//    createVertexJointIds(vt_jt_id, influenceJoints);
//    createVertexJointWeights(vt_jt_wt, weights);


    count = vt_IDX_COUNT;

    bufIdx.create();
    bufIdx.bind();
    bufIdx.setUsagePattern(QOpenGLBuffer::StaticDraw);
    bufIdx.allocate(vt_idx, vt_IDX_COUNT * sizeof(GLuint));

    bufPos.create();
    bufPos.bind();
    bufPos.setUsagePattern(QOpenGLBuffer::StaticDraw);
    bufPos.allocate(vt_vert_pos, vt_VERT_COUNT * sizeof(glm::vec4));

    bufCol.create();
    bufCol.bind();
    bufCol.setUsagePattern(QOpenGLBuffer::StaticDraw);
    bufCol.allocate(vt_vert_col, vt_VERT_COUNT * sizeof(glm::vec4));

    bufNor.create();
    bufNor.bind();
    bufNor.setUsagePattern(QOpenGLBuffer::StaticDraw);
    bufNor.allocate(vt_vert_nor, vt_VERT_COUNT * sizeof(glm::vec4));

    bufJointId.create();
    bufJointId.bind();
    bufJointId.setUsagePattern(QOpenGLBuffer::StaticDraw);
    bufJointId.allocate(vt_jt_id, vt_INFL_COUNT * sizeof(GLuint));

    bufJointWeight.create();
    bufJointWeight.bind();
    bufJointWeight.setUsagePattern(QOpenGLBuffer::StaticDraw);
    bufJointWeight.allocate(vt_jt_wt, vt_INFL_COUNT * sizeof(GLuint));

}

void Vertex::destroy()
{
    bufIdx.destroy();
    bufPos.destroy();
    bufNor.destroy();
    bufCol.destroy();
    bufJointWeight.destroy();
    bufJointId.destroy();
}

GLenum Vertex::drawMode()
{
    return GL_LINES;
}

int Vertex::elemCount()
{
    return count;
}

bool Vertex::bindIdx()
{
    return bufIdx.bind();
}

bool Vertex::bindPos()
{
    return bufPos.bind();
}

bool Vertex::bindNor()
{
    return bufNor.bind();
}

bool Vertex::bindCol()
{
    return bufCol.bind();
}



glm::ivec2 Vertex::getInfluenceJoints() const
{
    return influenceJoints;
}

void Vertex::setInfluenceJoints(const glm::ivec2 &value)
{
    influenceJoints = value;
}

glm::vec2 Vertex::getWeights() const
{
    return weights;
}

void Vertex::setWeights(const glm::vec2 &value)
{
    weights = value;
}




HalfEdge* Vertex::getEdge() {
    return this->edge;
}

int Vertex::getId() {
    return this->id;
}

glm::vec4 Vertex::getNor() {
    return this->nor;
}

bool Vertex::getWasSmoothed() {
    return this->wasSmoothed;
}

bool Vertex::getSharp() {
    return this->isSharp;
}

void Vertex::setSharp(bool s) {
    this->isSharp = s;
}

void Vertex::setPos(glm::vec4 p) {
    this->pos = p;
}

void Vertex::setEdge(HalfEdge* e) {
    this->edge = e;
}

void Vertex::setId(int i) {
    this->id = i;
    this->setText(QString::number(i));
}

void Vertex::setNor(glm::vec4 n) {
    this->nor = n;
}

void Vertex::setWasSmoothed(bool b) {
    this->wasSmoothed = b;
}


