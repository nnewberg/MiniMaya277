#pragma once

#include <shaderprogram.h>

#include <QOpenGLContext>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>

class Point : public ShaderProgram::Drawable
{
private:
    int count;
    glm::vec4 point_pos;
    QOpenGLBuffer bufIdx;
    QOpenGLBuffer bufPos;
    QOpenGLBuffer bufNor;
    QOpenGLBuffer bufCol;  // Can be used to pass per-vertex color information to the shader, but is currently unused.

public:
    Point();

    void create(glm::vec4 pos);
    void destroy();

    void setPointPos(glm::vec4 p);

    virtual GLenum drawMode();
    virtual int elemCount();
    virtual bool bindIdx();
    virtual bool bindPos();
    virtual bool bindNor();
    virtual bool bindCol();
    virtual bool bindIds();
    virtual bool bindWts();
};
