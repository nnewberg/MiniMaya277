#include "halfedge.h"

HalfEdge::HalfEdge()
{
    face = NULL;
    vert = NULL;
    next = NULL;
    sym = NULL;
    id = 0;
    wasSubdiv = false;
    isSharp = false;
}

Face* HalfEdge::getFace() {
    return this->face;
}

Vertex* HalfEdge::getVert(){
    return this->vert;
}

HalfEdge* HalfEdge::getNext(){
    return this->next;
}

HalfEdge* HalfEdge::getSym(){
    return this->sym;
}

int HalfEdge::getId(){
    return this->id;
}

bool HalfEdge::getWasSubdiv(){
    return this->wasSubdiv;
}

bool HalfEdge::getSharp() {
    return this->isSharp;
}

void HalfEdge::setSharp(bool s) {
    this->isSharp = s;
}

void HalfEdge::setFace(Face* f){
    this->face = f;
}

void HalfEdge::setVert(Vertex* v){
    this->vert = v;
}

void HalfEdge::setNext(HalfEdge* n){
    this->next = n;
}

void HalfEdge::setSym(HalfEdge* s) {
    this->sym = s;
}

void HalfEdge::setId(int i) {
    this->id = i;
    this->setText(QString::number(i));
}

void HalfEdge::setWasSubdiv(bool b) {
    this->wasSubdiv = b;
}
