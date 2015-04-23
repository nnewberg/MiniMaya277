#ifndef WIREBOX_H
#define WIREBOX_H
#include <shaderprogram.h>
#include "ray.h"
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

 public:
  wirebox();
  float intersect(ray r, glm::mat4 m);
  void create();
  void destroy();

  virtual GLenum drawMode();
  virtual int elemCount();
  virtual bool bindIdx();
  virtual bool bindPos();
  virtual bool bindNor();
  virtual bool bindCol();
  virtual bool bindIds();
  virtual bool bindWts();
};


#endif // WIREBOX_H

