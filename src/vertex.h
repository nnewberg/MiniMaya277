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

    glm::vec4 getPoint_pos() const;
    void setPoint_pos(const glm::vec4 &value);

    glm::vec4 getDefault_pos() const;
    void setDefault_pos(const glm::vec4 &value);

private:

    int count;
    glm::vec4 point_pos;
    glm::vec4 default_pos;

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
