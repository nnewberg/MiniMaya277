#include "face.h"
#include <iostream>
#include "halfedge.h"
#include "vertex.h"

Face::Face()
{
    start_edge = NULL;
    color = glm::vec4(0,1,0,1);
    id = 0;
    normal = glm::vec4(0);
    originalVertices = {};
    isSharp = false;
}

HalfEdge* Face::getStartEdge() {
    return this->start_edge;
}

glm::vec4 Face::getColor() {
    return this->color;
}

int Face::getId() {
    return this->id;
}

glm::vec4 Face::getNormal() {
    return this->normal;
}

Vertex* Face::getCentroid() {
    return this->centroid;
}

std::vector<Vertex*> Face::getOriginalVertices() {
    return this->originalVertices;
}

bool Face::getSharp() {
    return this->isSharp;
}

void Face::setSharp(bool s) {
    this->isSharp = s;
}

void Face::setStartEdge(HalfEdge* e){
    this->start_edge = e;
}

void Face::setColor(glm::vec4 c) {
    this->color = c;
}

void Face::setId(int i) {
    this->id = i;
    this->setText(QString::number(i));
}

void Face::setNormal(glm::vec4 n) {
    this->normal = n;
}

void Face::setCentroid(Vertex* c) {
    this->centroid = c;
}

void Face::setOriginalVertices(std::vector<Vertex*> v) {
    this->originalVertices = v;
}

std::vector<Vertex*> Face::getVertices(){
    HalfEdge* e = this->start_edge;
    HalfEdge* s = this->start_edge;
    HalfEdge* p = this->start_edge;
//    int eId = e->getId();
    std::vector<Vertex*> verts = {};
    verts.push_back(e->getVert());
    e = e->getNext();
    int i = 0;
    while (e->getId() != s->getId()) {
        if (e == NULL) {
            if (p != NULL) { std::cout << p->getId() << std::endl; }
            std::cout << "FACE ERROR: NULL HALFEDGE" << std::endl;
            break;
        }
        i ++;
        verts.push_back(e->getVert());
        p = e;
        e = e->getNext();
    }
    return verts;
}

void Face::printEdges(){
    HalfEdge* e = this->start_edge;
    int eId = e->getId();
    e = e->getNext();
//    std::cout << this->getId() << ": " << this->start_edge->getId() << " ";
    while (e->getId() != eId) {
        if (e == NULL) {
            std::cout << "NULL HALFEDGE" << std::endl;
            break;
        }
        e = e->getNext();
//        std::cout << e->getId() << " ";
    }
    std::cout << std::endl;
}

//int getSize(Face f){
//    int numSides = 1;
//    HalfEdge* e = f.start_edge->next;
//    while(e != f.start_edge) {
//        numSides++;
//        e = e->next;
//    }
//    return numSides;
//}

