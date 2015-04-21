#ifndef HALFEDGE_H
#define HALFEDGE_H
#include "face.h"
#include "vertex.h"
#include <QListWidgetItem>

class HalfEdge : public QListWidgetItem
{
public:
    HalfEdge();
    Face* getFace();
    Vertex* getVert();
    HalfEdge* getNext();
    HalfEdge* getSym();
    int getId();
    bool getWasSubdiv();
    bool getSharp();
    void setSharp(bool s);

    void setFace(Face* f);
    void setVert(Vertex* v);
    void setNext(HalfEdge* n);
    void setSym(HalfEdge* s);
    void setId(int i);
    void setWasSubdiv(bool b);


private:
    Face* face;
    Vertex* vert;
    HalfEdge* next;
    HalfEdge* sym;
    int id;
    bool wasSubdiv;
    bool isSharp;
};

#endif // HALFEDGE_H
