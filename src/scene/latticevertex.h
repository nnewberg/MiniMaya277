#ifndef LATTICEVERTEX_H
#define LATTICEVERTEX_H

#include <la.h>
#include <QListWidgetItem>
#include <QMap>
#include "joint.h"
#include "shaderprogram.h"
#include <QOpenGLContext>
#include <QOpenGLBuffer>
#include "vertex.h"


class HalfEdge;

class LatticeVertex : public QListWidgetItem
{
public:
    LatticeVertex();

    glm::vec4 getPosition() const;
    void setPosition(glm::vec4 &value);

    std::vector<Vertex *> getLatticeVertices() const;
    void setLatticeVertices(const std::vector<Vertex *> &value);
    void addVertex(Vertex* v);

    ShaderProgram::Drawable *getSphere();
    void setSphere(ShaderProgram::Drawable *value);

    glm::mat4 getTransformationMatrix() const;
    void setTransformationMatrix(const glm::mat4 &value);

    int getXId() const;
    void setXId(int value);

    int getYId() const;
    void setYId(int value);

    int getZId() const;
    void setZId(int value);

    glm::mat4 getDefaultTransformationMatrix() const;
    void setDefaultTransformationMatrix(const glm::mat4 &value);

    glm::vec4 getDefaultPosition() const;
    void setDefaultPosition(const glm::vec4 &value);

private:
    glm::vec4 position;
    std::vector<Vertex*> latticeVertices;
    ShaderProgram::Drawable *sphere;
    glm::mat4 transformationMatrix;

    glm::vec4 defaultPosition;
    glm::mat4 defaultTransformationMatrix;


    int xId;
    int yId;
    int zId;

};

#endif // LatticeVertex_H
