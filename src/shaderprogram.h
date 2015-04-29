#pragma once

#include <glwidget277.h>
#include <la.h>

#include <QOpenGLFunctions_3_2_Core>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>


class ShaderProgram
{
public:
    // This defines a class which can be rendered by our shader program.
    // Make any geometry a subclass of ShaderProgram::Drawable in order to render it with the ShaderProgram class.
    class Drawable
    {
    public:
        virtual GLenum drawMode() = 0;
        virtual int elemCount() = 0;
        virtual bool bindIdx() = 0;
        virtual bool bindPos() = 0;
        virtual bool bindNor() = 0;
        virtual bool bindCol() = 0;
        virtual bool bindIds() = 0;
        virtual bool bindWts() = 0;
    };

public:
    QOpenGLShaderProgram prog;

    int attrPos;
    int attrNor;
    int attrCol;
    int attrIds;
    int attrWts;

    int unifModel;
    int unifModelInvTr;
    int unifViewProj;
    int unifColor;

    int unifBindMat;
    int unifJointTran;

    int unifOutlined;
    int unifCentroid;
    int unifViewDir;

public:
    void create(const char *vertfile, const char *fragfile);
    void setModelMatrix(const glm::mat4 &model);
    void setViewProjMatrix(const glm::mat4& vp);
    void draw(GLWidget277 &f, Drawable &d);
    void setBindingMatrix(std::vector<glm::mat4> bms);

    void setJointTransform(std::vector<glm::mat4> jts);

    void setOutlined(bool outlined);
    void setCentroid(glm::vec4 centroid);

    void setViewDir(glm::vec4 view_dir);


};
