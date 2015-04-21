#ifndef VERTEX_H
#define VERTEX_H

#include <la.h>
#include <QListWidgetItem>
#include <QMap>
#include "joint.h"
#include "shaderprogram.h"
#include <QOpenGLContext>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>


class HalfEdge;

class Vertex : public QListWidgetItem
{
public:
    Vertex();
    Vertex(glm::vec4 p);
    glm::vec4 getPos();
    HalfEdge* getEdge();
    int getId();
    glm::vec4 getNor();
    bool getWasSmoothed();
    bool getSharp();
    void setSharp(bool s);

    void setPos(glm::vec4 p);
    void setEdge(HalfEdge* e);
    void setId(int i);
    void setNor(glm::vec4 n);
    void setWasSmoothed(bool b);

    glm::ivec2 getInfluenceJoints() const;
    void setInfluenceJoints(const glm::ivec2 &value);

    glm::vec2 getWeights() const;
    void setWeights(const glm::vec2 &value);

    void create();
    void destroy();

    virtual GLenum drawMode();
    virtual int elemCount();
    virtual bool bindIdx();
    virtual bool bindPos();
    virtual bool bindNor();
    virtual bool bindCol();


private:

    int count;
    glm::vec4 point_pos;
    QOpenGLBuffer bufIdx;
    QOpenGLBuffer bufPos;
    QOpenGLBuffer bufNor;
    QOpenGLBuffer bufCol;
    QOpenGLBuffer bufJointId;
    QOpenGLBuffer bufJointWeight;


    glm::vec4 pos;
    HalfEdge* edge;
    int id;
    glm::vec4 nor;
    bool wasSmoothed;
    bool isSharp;

    glm::ivec2 influenceJoints;
    glm::vec2 weights;

};

#endif // VERTEX_H
