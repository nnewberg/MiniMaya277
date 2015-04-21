#pragma once

#include <shaderprogram.h>

#include <QOpenGLContext>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>

class WireSphere : public ShaderProgram::Drawable
{
private:
    int count;
    QOpenGLBuffer bufIdx;
    QOpenGLBuffer bufPos;
    QOpenGLBuffer bufNor;
    QOpenGLBuffer bufCol;  // Can be used to pass per-vertex color information to the shader, but is currently unused.

    glm::vec4 color;


public:
    WireSphere();


    void create();

    void update();

    void destroy();

    virtual GLenum drawMode();
    virtual int elemCount();
    virtual bool bindIdx();
    virtual bool bindPos();
    virtual bool bindNor();
    virtual bool bindCol();
    virtual bool bindIds();
    virtual bool bindWts();
    glm::vec4 getColor() const;
    void setColor(const glm::vec4 &value);
};
