#include "vertex.h"


Vertex::Vertex() {
    pos = glm::vec4(0);
    edge = NULL;
    id = 0;
    wasSmoothed = false;
    isSharp = false;
    influenceJoints = {};
}

Vertex::Vertex(glm::vec4 p) {
    pos = p;
    edge = NULL;
    id = 0;
    wasSmoothed = false;
    isSharp = false;
}

glm::vec4 Vertex::getPos() {
    return this->pos;
}



glm::vec4 Vertex::getPoint_pos() const
{
    return point_pos;
}

void Vertex::setPoint_pos(const glm::vec4 &value)
{
    point_pos = value;
}




glm::ivec2 Vertex::getInfluenceJoints() const
{
    return influenceJoints;
}

void Vertex::setInfluenceJoints(const glm::ivec2 &value)
{
    influenceJoints = value;
}

glm::vec2 Vertex::getWeights() const
{
    return weights;
}

void Vertex::setWeights(const glm::vec2 &value)
{
    weights = value;
}




HalfEdge* Vertex::getEdge() {
    return this->edge;
}

int Vertex::getId() {
    return this->id;
}

glm::vec4 Vertex::getNor() {
    return this->nor;
}

bool Vertex::getWasSmoothed() {
    return this->wasSmoothed;
}

bool Vertex::getSharp() {
    return this->isSharp;
}

void Vertex::setSharp(bool s) {
    this->isSharp = s;
}

void Vertex::setPos(glm::vec4 p) {
    this->pos = p;
}

void Vertex::setEdge(HalfEdge* e) {
    this->edge = e;
}

void Vertex::setId(int i) {
    this->id = i;
    this->setText(QString::number(i));
}

void Vertex::setNor(glm::vec4 n) {
    this->nor = n;
}

void Vertex::setWasSmoothed(bool b) {
    this->wasSmoothed = b;
}


