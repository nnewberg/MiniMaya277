#include "joint.h"
#include <stdio.h>
#include <iostream>

std::vector<Joint*> Joint::globalJoints;

Joint::Joint()
{
    position = glm::vec3(0,0,0);
    rotation = glm::fquat(0,0,0,0);
    children = {};
    parent = NULL;
    distToVertex = 0;
    color = glm::vec4(1,1,1,1);
    id = 0;

}

glm::mat4 Joint::getLocalTransformation() {
    glm::mat4 positionMat = glm::translate(glm::mat4(), this->position);

    glm::mat4 rotationMat = glm::toMat4(rotation);

    return positionMat*rotationMat;
}

glm::mat4 Joint::getOverallTransformation() {
    Joint * c = this;
    glm::mat4 t = c->getLocalTransformation();
    if (c->getParent() != NULL) {
        Joint * p = (Joint*) c->getParent();
        t = p->getOverallTransformation()*t;
    }

    return t;
}



glm::mat4 Joint::getBindMatrix() const
{
    return bindMatrix;
}

void Joint::setBindMatrix(const glm::mat4 &value)
{
    bindMatrix = value;
}
glm::vec3 Joint::getPosition() const
{
    return position;
}

void Joint::setPosition(const glm::vec3 &value)
{
    position = value;
}
std::vector<QTreeWidgetItem *> Joint::getChildren() const
{
    return children;
}

void Joint::addChild(QTreeWidgetItem *child) {
    QTreeWidgetItem::addChild(child);
    children.push_back(child);
    Joint* c = (Joint*) child;
    c->setParent(this);
}

void Joint::setChildren(const std::vector<QTreeWidgetItem *> &value)
{
    children = value;
}
QTreeWidgetItem* Joint::getParent() const
{
    return parent;
}

void Joint::setParent(Joint *value)
{
    parent = value;
}
QString Joint::getName() const
{
    return name;
}

void Joint::setName(const QString &value)
{
    name = value;
    this->setText(0, value);
}
glm::fquat Joint::getRotation() const
{
    return rotation;
}

void Joint::setRotation(const glm::fquat &value)
{
    rotation = value;
}

glm::fquat Joint::createRotationQuat(float a, float x, float y, float z) {
    float w = cos(a/2);
    float qx = sin(a/2)*x;
    float qy = sin(a/2)*y;
    float qz = sin(a/2)*z;
    glm::fquat q = glm::fquat(w, qx, qy, qz);
    return q;

}
float Joint::getDistToVertex() const
{
    return distToVertex;
}

void Joint::setDistToVertex(float value)
{
    distToVertex = value;
}

bool Joint::operator<(const Joint &j2) const {
//    std::cout << "dists: " << this->getDistToVertex() << " " << j2.getDistToVertex() << std::endl;
    return this->getDistToVertex() < j2.getDistToVertex();
}
glm::vec4 Joint::getColor() const
{
    return color;
}

void Joint::setColor(const glm::vec4 &value)
{
    color = value;
}
int Joint::getId() const
{
    return id;
}

void Joint::setId(int value)
{
    id = value;
}










