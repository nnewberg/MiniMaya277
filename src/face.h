#ifndef FACE_H
#define FACE_H

#include <vector>
#include <QListWidgetItem>
#include "la.h"

class HalfEdge;
class Vertex;

class Face : public QListWidgetItem
{
public:
    Face();
    HalfEdge* getStartEdge();
    glm::vec4 getColor();
    int getId();
    glm::vec4 getNormal();
    Vertex* getCentroid();
    std::vector<Vertex*> getOriginalVertices();
    bool getSharp();
    void setSharp(bool s);


    void setStartEdge(HalfEdge* e);
    void setColor(glm::vec4 c);
    void setId(int i);
    void setNormal(glm::vec4 n);
    void setCentroid(Vertex* c);
    void setOriginalVertices(std::vector<Vertex*> v);



    std::vector<Vertex*> getVertices();
    void printEdges();


private:
    HalfEdge* start_edge;
    glm::vec4 color;
    int id;
    glm::vec4 normal;
    Vertex* centroid;
    int numVertices;
    std::vector<Vertex*> originalVertices;
    bool isSharp;




};

#endif // FACE_H
