#ifndef RAY_H
#define RAY_H

#include <shaderprogram.h>
#include <la.h>

#include <QOpenGLContext>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>

class ray : public ShaderProgram::Drawable
{
 private:
  int count;

  QOpenGLBuffer bufIdx;
  QOpenGLBuffer bufPos;
  QOpenGLBuffer bufCol; // Can be used to pass per-vertex color information to the shader, but is currently unused.
  QOpenGLBuffer bufNor;

 public:
  glm::vec4 ray_origin;
  glm::vec4 ray_direction;
  ray();
  ray(glm::vec4 eye, glm::vec4 p);

  void setRay(glm::vec4 eye, glm::vec4 p);

  void createRayVertexPositions(glm::vec4 (&ray_vert_pos)[2]);
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


#endif // RAY_H
