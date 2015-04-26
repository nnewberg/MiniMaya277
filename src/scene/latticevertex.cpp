#include "LatticeVertex.h"
#include <iostream>

LatticeVertex::LatticeVertex()
{
    position = glm::vec4(0);
    latticeVertices = {};

}
glm::vec4 LatticeVertex::getPosition() const
{
    return position;
}

void LatticeVertex::setPosition(glm::vec4 &value)
{
    position = value;

}
std::vector<Vertex *> LatticeVertex::getLatticeVertices() const
{
    return latticeVertices;
}

void LatticeVertex::setLatticeVertices(const std::vector<Vertex *> &value)
{
    latticeVertices = value;
}

void LatticeVertex::addVertex(Vertex* v) {
    latticeVertices.push_back(v);
}

ShaderProgram::Drawable *LatticeVertex::getSphere()
{
    return sphere;
}

void LatticeVertex::setSphere(ShaderProgram::Drawable *value)
{
    sphere = value;
}
glm::mat4 LatticeVertex::getTransformationMatrix() const
{
    return transformationMatrix;
}

void LatticeVertex::setTransformationMatrix(const glm::mat4 &value)
{
    transformationMatrix = value;
}
int LatticeVertex::getXId() const
{
    return xId;
}

void LatticeVertex::setXId(int value)
{
    xId = value;
}
int LatticeVertex::getYId() const
{
    return yId;
}

void LatticeVertex::setYId(int value)
{
    yId = value;
}
int LatticeVertex::getZId() const
{
    return zId;
}

void LatticeVertex::setZId(int value)
{
    zId = value;
}
glm::mat4 LatticeVertex::getDefaultTransformationMatrix() const
{
    return defaultTransformationMatrix;
}

void LatticeVertex::setDefaultTransformationMatrix(const glm::mat4 &value)
{
    defaultTransformationMatrix = value;
}
glm::vec4 LatticeVertex::getDefaultPosition() const
{
    return defaultPosition;
}

void LatticeVertex::setDefaultPosition(const glm::vec4 &value)
{
    defaultPosition = value;
}















