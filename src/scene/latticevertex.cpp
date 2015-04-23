#include "LatticeVertex.h"

static const int latvt_IDX_COUNT = 1;
static const int latvt_VERT_COUNT = 1;
static const int latvt_JOINT_COUNT = 2;
static const int latvt_INFL_COUNT = 2;



LatticeVertex::LatticeVertex()
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

LatticeVertex::LatticeVertex(glm::vec4 p) {
    pos = p;
    edge = NULL;
    id = 0;
    wasSmoothed = false;
    isSharp = false;
}

glm::vec4 LatticeVertex::getPos() {
    return this->pos;
}


//need to make QOpenGLBuffer for all four shader attributes
//create
// shaderprogram - link up memory

void createLatticeVertexLatticeVertexPositions(glm::vec4 (&latvt_vert_pos)[latvt_VERT_COUNT], glm::vec4 position)
{
    latvt_vert_pos[0] = position;
}

void createLatticeVertexLatticeVertexNormals(glm::vec4 (&latvt_vert_nor)[latvt_VERT_COUNT])
{
    latvt_vert_nor[0] = glm::vec4(0,0,0,0);
}

void createLatticeVertexColors(glm::vec4 (&latvt_vert_col)[latvt_VERT_COUNT])
{
    glm::vec4 c = glm::vec4(1,1,0,1);
    latvt_vert_col[0] = c;
}


void createLatticeVertexIndices(GLuint (&latvt_idx)[latvt_IDX_COUNT])
{
    latvt_idx[0] = 0;
}


//void createLatticeVertexJointIds(GLuint (&latvt_jt_id)[latvt_INFL_COUNT], glm::vec2 joints) {
//    latvt_jt_id[0] = joints[0];
//    latvt_jt_id[1] = joints[1];
//}

//void createLatticeVertexJointWeights(GLuint (&latvt_jt_wt)[latvt_INFL_COUNT], glm::vec2 weights) {
//    latvt_jt_wt[0] = weights[0];
//    latvt_jt_wt[1] = weights[1];
//}

void LatticeVertex::create()
{
    GLuint latvt_idx[latvt_IDX_COUNT];
    glm::vec4 latvt_vert_pos[latvt_VERT_COUNT];
    glm::vec4 latvt_vert_nor[latvt_VERT_COUNT];
    glm::vec4 latvt_vert_col[latvt_VERT_COUNT];

    GLuint latvt_jt_id[latvt_INFL_COUNT];
    GLuint latvt_jt_wt[latvt_INFL_COUNT];

    createLatticeVertexLatticeVertexPositions(latvt_vert_pos, pos);
    createLatticeVertexLatticeVertexNormals(latvt_vert_nor);
    createLatticeVertexColors(latvt_vert_col);
    createLatticeVertexIndices(latvt_idx);

//    createLatticeVertexJointIds(latvt_jt_id, influenceJoints);
//    createLatticeVertexJointWeights(latvt_jt_wt, weights);


    count = latvt_IDX_COUNT;

    bufIdx.create();
    bufIdx.bind();
    bufIdx.setUsagePattern(QOpenGLBuffer::StaticDraw);
    bufIdx.allocate(latvt_idx, latvt_IDX_COUNT * sizeof(GLuint));

    bufPos.create();
    bufPos.bind();
    bufPos.setUsagePattern(QOpenGLBuffer::StaticDraw);
    bufPos.allocate(latvt_vert_pos, latvt_VERT_COUNT * sizeof(glm::vec4));

    bufCol.create();
    bufCol.bind();
    bufCol.setUsagePattern(QOpenGLBuffer::StaticDraw);
    bufCol.allocate(latvt_vert_col, latvt_VERT_COUNT * sizeof(glm::vec4));

    bufNor.create();
    bufNor.bind();
    bufNor.setUsagePattern(QOpenGLBuffer::StaticDraw);
    bufNor.allocate(latvt_vert_nor, latvt_VERT_COUNT * sizeof(glm::vec4));

    bufJointId.create();
    bufJointId.bind();
    bufJointId.setUsagePattern(QOpenGLBuffer::StaticDraw);
    bufJointId.allocate(latvt_jt_id, latvt_INFL_COUNT * sizeof(GLuint));

    bufJointWeight.create();
    bufJointWeight.bind();
    bufJointWeight.setUsagePattern(QOpenGLBuffer::StaticDraw);
    bufJointWeight.allocate(latvt_jt_wt, latvt_INFL_COUNT * sizeof(GLuint));

}

void LatticeVertex::destroy()
{
    bufIdx.destroy();
    bufPos.destroy();
    bufNor.destroy();
    bufCol.destroy();
    bufJointWeight.destroy();
    bufJointId.destroy();
}

GLenum LatticeVertex::drawMode()
{
    return GL_LINES;
}

int LatticeVertex::elemCount()
{
    return count;
}

bool LatticeVertex::bindIdx()
{
    return bufIdx.bind();
}

bool LatticeVertex::bindPos()
{
    return bufPos.bind();
}

bool LatticeVertex::bindNor()
{
    return bufNor.bind();
}

bool LatticeVertex::bindCol()
{
    return bufCol.bind();
}
glm::vec4 LatticeVertex::getPoint_pos() const
{
    return point_pos;
}

void LatticeVertex::setPoint_pos(const glm::vec4 &value)
{
    point_pos = value;
}




glm::ivec2 LatticeVertex::getInfluenceJoints() const
{
    return influenceJoints;
}

void LatticeVertex::setInfluenceJoints(const glm::ivec2 &value)
{
    influenceJoints = value;
}

glm::vec2 LatticeVertex::getWeights() const
{
    return weights;
}

void LatticeVertex::setWeights(const glm::vec2 &value)
{
    weights = value;
}




HalfEdge* LatticeVertex::getEdge() {
    return this->edge;
}

int LatticeVertex::getId() {
    return this->id;
}

glm::vec4 LatticeVertex::getNor() {
    return this->nor;
}

bool LatticeVertex::getWasSmoothed() {
    return this->wasSmoothed;
}

bool LatticeVertex::getSharp() {
    return this->isSharp;
}

void LatticeVertex::setSharp(bool s) {
    this->isSharp = s;
}

void LatticeVertex::setPos(glm::vec4 p) {
    this->pos = p;
}

void LatticeVertex::setEdge(HalfEdge* e) {
    this->edge = e;
}

void LatticeVertex::setId(int i) {
    this->id = i;
    this->setText(QString::number(i));
}

void LatticeVertex::setNor(glm::vec4 n) {
    this->nor = n;
}

void LatticeVertex::setWasSmoothed(bool b) {
    this->wasSmoothed = b;
}


