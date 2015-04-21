#ifndef OBJREADER_H
#define OBJREADER_H

#include <QFile>
#include <QDialog>
#include <QFileDialog>
#include <QTextStream>
#include <stdio.h>
#include <iostream>
#include <vertex.h>
#include <face.h>
#include <halfedge.h>
#include "la.h"
#include <QMap>
#include <tuple>
#include <utility>

class ObjReader : QDialog
{
public:
    ObjReader();
    QFileDialog fileDialog;
    void reset();
    bool readFile();

    void handleVertices(std::string s);
    void handleNormals(std::string s);
    void handleFaces(QString s);
    void setSymEdges();

    std::vector<QListWidgetItem*> getVertices();
    std::vector<QListWidgetItem*> getEdges();
    std::vector<glm::vec4> getOrderedNormals();
    std::vector<QListWidgetItem*> getFaces();

private:
    QFile inFile;
    std::vector<QListWidgetItem*> vertices;
    std::vector<QListWidgetItem*> edges;
    std::vector<glm::vec4> normals;
    std::vector<glm::vec4> orderedNormals;
    std::vector<QListWidgetItem *> faces;

    QMap<std::tuple<int, int>, HalfEdge*> vertexEdges;
};

#endif // OBJREADER_H
