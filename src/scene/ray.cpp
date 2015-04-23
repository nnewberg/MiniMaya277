#include "ray.h"
#include<la.h>
#include<math.h>
#include <iostream>

static const int RAY_IDX_COUNT = 2;
static const int RAY_VERT_COUNT = 2;
glm::vec4 ray_origin;
glm::vec4 ray_direction;
glm::vec4 ray_vert_pos[RAY_VERT_COUNT];


ray::ray()
  : bufIdx(QOpenGLBuffer::IndexBuffer),
    bufPos(QOpenGLBuffer::VertexBuffer),
    bufCol(QOpenGLBuffer::VertexBuffer),
    bufNor(QOpenGLBuffer::VertexBuffer){
}


ray::ray(glm::vec4 eye, glm::vec4 p)
  : bufIdx(QOpenGLBuffer::IndexBuffer),
    bufPos(QOpenGLBuffer::VertexBuffer),
    bufCol(QOpenGLBuffer::VertexBuffer),
    bufNor(QOpenGLBuffer::VertexBuffer){

    this->ray_origin = eye;
    this->ray_direction = (p-eye)/(glm::length(p-eye));
//    this->ray_direction = d/(length(d));
//    this->rayPoint = p;
}

void ray::setRay(glm::vec4 eye, glm::vec4 d) {
    this->ray_origin = eye;
//    glm::vec4 d = p-eye;
    this->ray_direction = d;
    createRayVertexPositions(ray_vert_pos);
}

void ray::createRayVertexPositions(glm::vec4 (&ray_vert_pos)[RAY_VERT_COUNT]){
    ray_vert_pos[0] = ray_origin;
//    ray_direction[0]*50f;
//    ray_direction[1]*50f;
//    ray_direction[2]*50f;
//    ray_direction[3]*50f;
    ray_vert_pos[1] = ray_origin + glm::vec4(ray_direction[0]*50.0f, ray_direction[0]*50.0f, ray_direction[0]*50.0f, ray_direction[3]*50.0f);
//    std::cout<< ray_vert_pos[0] << " " << ray_vert_pos[1] << std::endl;
}

void createRayIndices(GLuint (&ray_idx)[RAY_IDX_COUNT]){
    ray_idx[0] = 0;
    ray_idx[1] = 1;
}


void ray::create()
{
  GLuint ray_idx[RAY_IDX_COUNT];

  createRayVertexPositions(ray_vert_pos);
  createRayIndices(ray_idx);

  count = RAY_IDX_COUNT;

  bufIdx.create();
  bufIdx.bind();
  bufIdx.setUsagePattern(QOpenGLBuffer::StaticDraw);
  bufIdx.allocate(ray_idx, RAY_IDX_COUNT * sizeof(GLuint));

  bufPos.create();
  bufPos.bind();
  bufPos.setUsagePattern(QOpenGLBuffer::StaticDraw);
  bufPos.allocate(ray_vert_pos, RAY_VERT_COUNT * sizeof(glm::vec4));

  bufNor.create();
  bufNor.bind();
  bufNor.setUsagePattern(QOpenGLBuffer::StaticDraw);
  bufNor.allocate(0, 0 * sizeof(glm::vec4));

}

void ray::destroy()
{
  bufIdx.destroy();
  bufPos.destroy();
  bufNor.destroy();
  bufCol.destroy();
}

GLenum ray::drawMode()
{
  return GL_LINES;
}

int ray::elemCount()
{
  return count;
}

bool ray::bindIdx()
{
  return bufIdx.bind();
}

bool ray::bindPos()
{
  return bufPos.bind();
}

bool ray::bindNor()
{
  return bufNor.bind();
}

bool ray::bindCol()
{
  return bufCol.bind();
}
