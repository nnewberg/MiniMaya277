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
    void setPosition(const glm::vec4 &value);

    std::vector<Vertex *> getLatticeVertices() const;
    void setLatticeVertices(const std::vector<Vertex *> &value);


private:
    glm::vec4 position;
    std::vector<Vertex*> latticeVertices;

};

#endif // LatticeVertex_H
