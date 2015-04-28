#include "objreader.h"


ObjReader::ObjReader()
{

}

void ObjReader::reset(){
    vertices = {};
    edges = {};
    normals = {};
    orderedNormals = {};
    faces = {};


}

bool ObjReader::readFile(){

    QString filename = QFileDialog::getOpenFileName(0, QString(""), QString("../../"), QString("*.obj"));

    if (filename.size() <= 0) {
        std::cout << "no file specified" << std::endl;
        return false;
    }
    if (!filename.endsWith(".obj",Qt::CaseSensitive)) {
        std::cout << "invalid filetype" << std::endl;
        return false;
    }

    QFile objFile(filename);
    if (!objFile.exists()) {
        std::cout << "file doesnt exist" << std::endl;
        return false;
    }
    objFile.open(QIODevice::ReadOnly);
    reset();
    QTextStream in(&objFile);
        while (!in.atEnd()) {
            QString line = in.readLine();
            std::string lineText = line.toUtf8().constData();
            if (lineText.size() > 2) {
                if (lineText.at(0) == 'v' && lineText.at(1) == ' ') {
                    handleVertices(lineText);
                }
                else if (lineText.at(0) == 'v' && lineText.at(1) == 't') {
                    continue; // ignore textures for now
                }
                else if (lineText.at(0) == 'v' && lineText.at(1) == 'n') {
                    handleNormals(lineText);
                }
                else if (lineText.at(0) == 'f' && lineText.at(1) == ' ') {
                    handleFaces(line);
                }
                else {
                    continue;
                }
            }
            else {
                continue;
            }
        }
        setSymEdges();
        objFile.close();
        return true;
}

void ObjReader::handleVertices(std::string s) {
    const char * line = s.c_str();
    char c;
    float x, y, z;
    sscanf(line, "%s %f %f %f", &c, &x, &y, &z);
    glm::vec4 vPosition = glm::vec4(x,y,z,1);
    Vertex * v = new Vertex();
    v->setPos(vPosition);
    v->setDefault_pos(vPosition);
    v->setPoint_pos(vPosition);
    v->setId(vertices.size());
    vertices.push_back(v);
//    std::cout << vertices.size() << std::endl;
}

void ObjReader::handleNormals(std::string s) {
    const char * line = s.c_str();
    char c[2];
    float x, y, z;
    sscanf(line, "%s %f %f %f", c, &x, &y, &z);
    glm::vec4 n = glm::vec4(x,y,z,0);
    normals.push_back(n);

}

void ObjReader::handleFaces(QString s) {
//    std::cout << "handle face" << std::endl;
    std::string lineText = s.toUtf8().constData();
    const char * line = lineText.c_str();

    Face* f = new Face();
    std::vector<int> faceVerts = {};
    std::vector<int> faceNors = {};
    std::vector<HalfEdge*> faceEdges = {};

    char substr[50];
    char sl[2];

    int v, tx, n, offset;
    line += 1;
    // store vertex/normals in vector
    // ignore texture for now
    while (sscanf(line, "%s%n", substr, &offset) == 1) {
        QString sub = QString::fromStdString(substr);
//        std::cout << substr << std::endl;
        // a//c format
        if (sub.contains("//", Qt::CaseInsensitive)) {
            int idx = sub.indexOf("//", 0);
            QString fv = sub.left(idx);
            QString sv = sub.mid(idx+2);
            QString vvv = sub.at(idx+2);
            bool ok;
            v = fv.toInt(&ok, 10);
            n = sv.toInt(&ok, 10);
            faceVerts.push_back(v);
            faceNors.push_back(n);
            line += offset;
        }
        // a/b/c and a/b format
        else if (sub.contains("/", Qt::CaseInsensitive)) {
            int idx = sub.indexOf("/", 0);
            int idx2 = sub.indexOf("/", idx+1);
            QString fv = sub.left(idx);
            bool ok;
            v = fv.toInt(&ok,10);

            if (idx2 > 0) {
                QString sv = sub.mid(idx2+1);
                n = sv.toInt(&ok,10);
                faceNors.push_back(n);
            }
            else {
                QString sv = sub.mid(idx+1);
                tx = sv.toInt(&ok,10);
            }

//            std::cout << "ints " << v << " " <<tx<< " " << n << std::endl;
            faceVerts.push_back(v);
            line += offset;
        }
        // a format
        else if (sscanf(substr, "%d", &v) == 1) {
            faceVerts.push_back(v);
//            std::cout << "h4" << std::endl;
            line += offset;
        }
        // offset < 3 => no normals specified
        if (offset < 3) {
            faceNors = {};
        }
    }

    // create face edges
    for (unsigned long i = 0; i < faceVerts.size(); i++) {
        int vIdx = faceVerts.at(i) - 1;
        Vertex* v = (Vertex*) vertices.at(vIdx);
        HalfEdge* e = new HalfEdge();
        e->setId(edges.size());
        edges.push_back(e);

        if (faceNors.size() == faceVerts.size()) {
            int nIdx = faceNors.at(i) - 1;
            orderedNormals.push_back(normals.at(nIdx));
            v->setNor(normals.at(nIdx));
        }
        e->setVert(v);
        e->setFace(f);
        v->setEdge(e);
        faceEdges.push_back(e);
    }
    f->setStartEdge(faceEdges.at(0));
    // set next pointers and put edges in tuple/edge map
    for (unsigned long i = 0; i < faceEdges.size()-1; i++) {
        faceEdges.at(i)->setNext(faceEdges.at(i+1));
        std::tuple<int, int> t (faceVerts.at(i)-1, faceVerts.at(i+1)-1);
        vertexEdges[t] = faceEdges.at(i+1);
    }
    // last edge in map
    std::tuple<int, int> t2 (faceVerts.at(faceVerts.size()-1) - 1, faceVerts.at(0)-1);

    vertexEdges[t2] = faceEdges.at(0);
    faceEdges.at(faceEdges.size()-1)->setNext(faceEdges.at(0));
    f->setId(faces.size());
    faces.push_back(f);
}

void ObjReader::setSymEdges(){
    QMapIterator<std::tuple<int, int>, HalfEdge*> i(vertexEdges);
    while (i.hasNext()) {
        i.next();
        int v0 = std::get<0>(i.key());
        int v1 = std::get<1>(i.key());

        std::tuple<int, int> sym (v1,v0);

        HalfEdge* e = i.value();
        if (vertexEdges.contains(sym)) {
            HalfEdge* eSym = vertexEdges.value(sym);
            e->setSym(eSym);
            eSym->setSym(e);
        }


    }
}

std::vector<QListWidgetItem*> ObjReader::getVertices(){
    return this->vertices;
}

std::vector<QListWidgetItem*> ObjReader::getEdges(){
    return this->edges;
}

std::vector<glm::vec4> ObjReader::getOrderedNormals(){
    return this->orderedNormals;
}

std::vector<QListWidgetItem *> ObjReader::getFaces(){
    return this->faces;
}
