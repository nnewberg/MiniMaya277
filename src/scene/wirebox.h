#ifndef WIREBOX_H
#define WIREBOX_H
#include <shaderprogram.h>
#include "ray.h"
#include "vertex.h"
#include <QOpenGLContext>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>

class wirebox : public ShaderProgram::Drawable
{
 private:
  int count;
  QOpenGLBuffer bufIdx;
  QOpenGLBuffer bufPos;
  QOpenGLBuffer bufNor;
  QOpenGLBuffer bufCol; // Can be used to pass per-vertex color information to the shader, but is currently unused.

  std::vector<Vertex*> boxVertices;

 public:
  wirebox();
  float intersect(ray r, glm::mat4 m);

  glm::mat4 transformationMatrix;

  void create();
  void destroy();

  void update();

  virtual GLenum drawMode();
  virtual int elemCount();
  virtual bool bindIdx();
  virtual bool bindPos();
  virtual bool bindNor();
  virtual bool bindCol();
  virtual bool bindIds();
  virtual bool bindWts();
  std::vector<Vertex *> getBoxVertices() const;
  void setBoxVertices(const std::vector<Vertex *> &value);
  glm::mat4 getTransformationMatrix() const;
  void setTransformationMatrix(const glm::mat4 &value);
};


#endif // WIREBOX_H

