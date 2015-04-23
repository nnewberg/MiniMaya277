#include "Lattice.h"

#include<la.h>
#include <iostream>



std::vector<Joint *> Lattice::getAllJoints() const
{
    return allJoints;
}

void Lattice::setAllJoints(const std::vector<Joint *> &value)
{
    allJoints = value;
}
Lattice::Lattice()
    : bufIdx(QOpenGLBuffer::IndexBuffer),
      bufPos(QOpenGLBuffer::VertexBuffer),
      bufNor(QOpenGLBuffer::VertexBuffer),
      bufCol(QOpenGLBuffer::VertexBuffer),
      bufJointId(QOpenGLBuffer::VertexBuffer),
      bufJointWeight(QOpenGLBuffer::VertexBuffer)
{

}


void Lattice::setVertices(std::vector<QListWidgetItem*> v) {
    this->vertices = v;
}

void Lattice::setEdges(std::vector<QListWidgetItem*> e) {
    this->edges = e;
}

void Lattice::setFaces(std::vector<QListWidgetItem*> f) {
    this->faces = f;
}

// set up vertices, halfedges, and faces for a cube [-0.5, 0.5]
void Lattice::createCube() {
    imported = false;
    // 8 vertices
    // 24 halfedges
    // 6 faces
    // top ring
    Vertex* v0 = new Vertex(glm::vec4(-0.5, -0.5, -0.5, 1));
    Vertex* v1 = new Vertex(glm::vec4(0.5, -0.5, -0.5, 1));
    Vertex* v2 = new Vertex(glm::vec4(0.5, -0.5, 0.5, 1));
    Vertex* v3 = new Vertex(glm::vec4(-0.5, -0.5, 0.5, 1));

    // bottom ring
    Vertex* v4 = new Vertex(glm::vec4(-0.5, 0.5, -0.5, 1));
    Vertex* v5 = new Vertex(glm::vec4(0.5, 0.5, -0.5, 1));
    Vertex* v6 = new Vertex(glm::vec4(0.5, 0.5, 0.5, 1));
    Vertex* v7 = new Vertex(glm::vec4(-0.5, 0.5, 0.5, 1));

    v0->setId(0);
    v1->setId(1);
    v2->setId(2);
    v3->setId(3);
    v4->setId(4);
    v5->setId(5);
    v6->setId(6);
    v7->setId(7);

    vertices.push_back(v0);
    vertices.push_back(v1);
    vertices.push_back(v2);
    vertices.push_back(v3);
    vertices.push_back(v4);
    vertices.push_back(v5);
    vertices.push_back(v6);
    vertices.push_back(v7);

    // create faces
    Face* f0 = new Face();
    Face* f1 = new Face();
    Face* f2 = new Face();
    Face* f3 = new Face();
    Face* f4 = new Face();
    Face* f5 = new Face();

    f0->setId(0);
    f1->setId(1);
    f2->setId(2);
    f3->setId(3);
    f4->setId(4);
    f5->setId(5);

    faces.push_back(f0);
    faces.push_back(f1);
    faces.push_back(f2);
    faces.push_back(f3);
    faces.push_back(f4);
    faces.push_back(f5);

    //create halfedges for each face

    // top
    HalfEdge* h01 = new HalfEdge();
    HalfEdge* h12 = new HalfEdge();
    HalfEdge* h23 = new HalfEdge();
    HalfEdge* h30 = new HalfEdge();

    // front
    HalfEdge* h03 = new HalfEdge();
    HalfEdge* h37 = new HalfEdge();
    HalfEdge* h74 = new HalfEdge();
    HalfEdge* h40 = new HalfEdge();

    // left
    HalfEdge* h32 = new HalfEdge();
    HalfEdge* h26 = new HalfEdge();
    HalfEdge* h67 = new HalfEdge();
    HalfEdge* h73 = new HalfEdge();

    // back
    HalfEdge* h21 = new HalfEdge();
    HalfEdge* h15 = new HalfEdge();
    HalfEdge* h56 = new HalfEdge();
    HalfEdge* h62 = new HalfEdge();

    // right
    HalfEdge* h10 = new HalfEdge();
    HalfEdge* h04 = new HalfEdge();
    HalfEdge* h45 = new HalfEdge();
    HalfEdge* h51 = new HalfEdge();

    // bottom
    HalfEdge* h47 = new HalfEdge();
    HalfEdge* h76 = new HalfEdge();
    HalfEdge* h65 = new HalfEdge();
    HalfEdge* h54 = new HalfEdge();

    // top
    edges.push_back(h01);
    edges.push_back(h12);
    edges.push_back(h23);
    edges.push_back(h30);

    // front
    edges.push_back(h03);
    edges.push_back(h37);
    edges.push_back(h74);
    edges.push_back(h40);

    // left
    edges.push_back(h32);
    edges.push_back(h26);
    edges.push_back(h67);
    edges.push_back(h73);

    // back
    edges.push_back(h21);
    edges.push_back(h15);
    edges.push_back(h56);
    edges.push_back(h62);

    // right
    edges.push_back(h10);
    edges.push_back(h04);
    edges.push_back(h45);
    edges.push_back(h51);

    // bottom
    edges.push_back(h47);
    edges.push_back(h76);
    edges.push_back(h65);
    edges.push_back(h54);

    for (unsigned long i = 0; i < edges.size(); i++) {
        HalfEdge * e = (HalfEdge * ) edges.at(i);
        e->setId(i);
    }

    // set face-halfedge pointers by face
    // top face
    f0->setStartEdge(h01);

    h01->setFace(f0);
    h12->setFace(f0);
    h23->setFace(f0);
    h30->setFace(f0);

    h01->setVert(v1);
    h12->setVert(v2);
    h23->setVert(v3);
    h30->setVert(v0);

    h01->setNext(h12);
    h12->setNext(h23);
    h23->setNext(h30);
    h30->setNext(h01);

    h01->setSym(h10);
    h12->setSym(h21);
    h23->setSym(h32);
    h30->setSym(h03);


    // front
    f1->setStartEdge(h03);

    h03->setFace(f1);
    h37->setFace(f1);
    h74->setFace(f1);
    h40->setFace(f1);

    h03->setVert(v3);
    h37->setVert(v7);
    h74->setVert(v4);
    h40->setVert(v0);

    h03->setNext(h37);
    h37->setNext(h74);
    h74->setNext(h40);
    h40->setNext(h03);

    h03->setSym(h30);
    h37->setSym(h73);
    h74->setSym(h47);
    h40->setSym(h04);


    // left
    f2->setStartEdge(h32);

    h32->setFace(f2);
    h26->setFace(f2);
    h67->setFace(f2);
    h73->setFace(f2);

    h32->setVert(v2);
    h26->setVert(v6);
    h67->setVert(v7);
    h73->setVert(v3);

    h32->setNext(h26);
    h26->setNext(h67);
    h67->setNext(h73);
    h73->setNext(h32);

    h32->setSym(h23);
    h26->setSym(h62);
    h67->setSym(h76);
    h73->setSym(h37);

    // back
    f3->setStartEdge(h21);

    h21->setFace(f3);
    h15->setFace(f3);
    h56->setFace(f3);
    h62->setFace(f3);

    h21->setVert(v1);
    h15->setVert(v5);
    h56->setVert(v6);
    h62->setVert(v2);

    h21->setNext(h15);
    h15->setNext(h56);
    h56->setNext(h62);
    h62->setNext(h21);

    h21->setSym(h12);
    h15->setSym(h51);
    h56->setSym(h65);
    h62->setSym(h26);

    // right
    f4->setStartEdge(h10);

    h10->setFace(f4);
    h04->setFace(f4);
    h45->setFace(f4);
    h51->setFace(f4);

    h10->setVert(v0);
    h04->setVert(v4);
    h45->setVert(v5);
    h51->setVert(v1);

    h10->setNext(h04);
    h04->setNext(h45);
    h45->setNext(h51);
    h51->setNext(h10);

    h10->setSym(h01);
    h04->setSym(h40);
    h45->setSym(h54);
    h51->setSym(h15);


    // bottom
    f5->setStartEdge(h47);

    h47->setFace(f5);
    h76->setFace(f5);
    h65->setFace(f5);
    h54->setFace(f5);

    h47->setVert(v7);
    h76->setVert(v6);
    h65->setVert(v5);
    h54->setVert(v4);

    h47->setNext(h76);
    h76->setNext(h65);
    h65->setNext(h54);
    h54->setNext(h47);

    h47->setSym(h74);
    h76->setSym(h67);
    h65->setSym(h56);
    h54->setSym(h45);


    // vertex-edge pointers
    v0->setEdge(h30);
    v1->setEdge(h01);
    v2->setEdge(h12);
    v3->setEdge(h23);
    v4->setEdge(h74);
    v5->setEdge(h45);
    v6->setEdge(h56);
    v7->setEdge(h67);

    //    calculateAllNormals();


}


// get number of sides to a face
//int getSize(Face* f){
//    int numSides = 1;
//    HalfEdge* e = f->getStartEdge()->getNext();
//    while(e != f->getStartEdge()) {
//        numSides++;
//        e = e->getNext();
//    }
//    return numSides;
//}



void createLatticeVertexPositions(std::vector<glm::vec4> *Lattice_vert_pos, std::vector<QListWidgetItem*> faces){
    int i = 0;
    for (unsigned long k = 0; k < faces.size(); k++) {
        Face* f = (Face*) faces.at(k);
        //        f->printEdges();
        std::vector<Vertex*> verts = f->getVertices();
        int faceSize = verts.size();
        //        std::cout << "faceSize " << faceSize << std::endl;
        for(int j = 0; j < faceSize; j++) {
            Vertex* v = verts.at(j);
            Lattice_vert_pos->push_back(v->getPos());
            v->setPoint_pos(v->getPos());
        }
        i += faceSize;
    }
}

void createLatticeIndices(std::vector<GLuint> *Lattice_idx, std::vector<QListWidgetItem*> faces){
    int i = 0;
    for (unsigned long k = 0; k < faces.size(); k++) {
        Face* f = (Face*) faces.at(k);
        // create line indices for wireframe cube
        Lattice_idx->push_back(i+0);
        Lattice_idx->push_back(i+1);
        Lattice_idx->push_back(i+1);
        Lattice_idx->push_back(i+2);
        Lattice_idx->push_back(i+2);
        Lattice_idx->push_back(i+3);
        Lattice_idx->push_back(i+3);
        Lattice_idx->push_back(i+0);
        i += 4;
    }
}

void createLatticeVertexNormals(std::vector<glm::vec4> *Lattice_vert_nor, std::vector<QListWidgetItem*> faces, std::vector<glm::vec4> norms, bool imported){
    int i = 0;
    for (unsigned long k = 0; k < faces.size(); k++) {
        Face* f = (Face*) faces.at(k);
        //        if (!imported) {
        //            calculateNormal(f);
        //        }
        // don't care about normals for lattice
        std::vector<Vertex*> verts = f->getVertices();
        int faceSize = verts.size();
        for(int j = 0; j < faceSize; j++) {
            Lattice_vert_nor->push_back(glm::vec4(0));
        }
        i += faceSize;
    }

}

void createLatticeColors(std::vector<glm::vec4> *Lattice_vert_col, std::vector<QListWidgetItem*> faces){
    int i = 0;
    glm::vec4 color = glm::vec4(1,1,1,1);
    for (unsigned long k = 0; k < faces.size(); k++) {
        Face* f = (Face*) faces.at(k);
        std::vector<Vertex*> verts = f->getVertices();
        int faceSize = verts.size();
        for(int j = 0; j < faceSize; j++) {
            Lattice_vert_col->push_back(color);
        }
        i += faceSize;
    }
}

void Lattice::create()
{

    setWasVertexDeleted(false);
    std::vector<glm::vec4> Lattice_vert_pos = {};
    std::vector<glm::vec4> Lattice_vert_col = {};
    std::vector<glm::vec4> Lattice_vert_nor = {};
    std::vector<GLuint> Lattice_idx;

    std::vector<glm::ivec2> Lattice_joint_ids = {};
    std::vector<glm::vec2> Lattice_joint_weights = {};

    edges = {};
    faces = {};
    vertices = {};

    createCube();


    count = Lattice_idx.size();

    bufIdx.create();
    bufIdx.bind();
    bufIdx.setUsagePattern(QOpenGLBuffer::StaticDraw);
    bufIdx.allocate(Lattice_idx.data(), Lattice_idx.size() * sizeof(GLuint));

    bufPos.create();
    bufPos.bind();
    bufPos.setUsagePattern(QOpenGLBuffer::StaticDraw);
    bufPos.allocate(Lattice_vert_pos.data(), Lattice_vert_pos.size() * sizeof(glm::vec4));

    bufCol.create();
    bufCol.bind();
    bufCol.setUsagePattern(QOpenGLBuffer::StaticDraw);
    bufCol.allocate(Lattice_vert_col.data(), Lattice_vert_col.size() * sizeof(glm::vec4));

    bufNor.create();
    bufNor.bind();
    bufNor.setUsagePattern(QOpenGLBuffer::StaticDraw);
    bufNor.allocate(Lattice_vert_nor.data(), Lattice_vert_nor.size() * sizeof(glm::vec4));

    bufJointId.create();
    bufJointId.bind();
    bufJointId.setUsagePattern(QOpenGLBuffer::StaticDraw);
    bufJointId.allocate(Lattice_joint_ids.data(), Lattice_joint_ids.size() * sizeof(glm::ivec2));

    bufJointWeight.create();
    bufJointWeight.bind();
    bufJointWeight.setUsagePattern(QOpenGLBuffer::StaticDraw);
    bufJointWeight.allocate(Lattice_joint_weights.data(), Lattice_joint_weights.size() * sizeof(glm::vec2));

    maxNumVerts = vertices.size();
    maxNumEdges = edges.size();
    maxNumFaces = faces.size();



    updateLattice();
}

void Lattice::updateLattice() {
    std::vector<glm::vec4> Lattice_vert_pos = {};
    std::vector<glm::vec4> Lattice_vert_col = {};
    std::vector<glm::vec4> Lattice_vert_nor = {};
    std::vector<GLuint> Lattice_idx = {};

    std::vector<glm::ivec2> Lattice_joint_ids = {};
    std::vector<glm::vec2> Lattice_joint_weights = {};

    if (faces.size() > (unsigned long) maxNumFaces) {
        maxNumFaces = faces.size();
    }
    if (vertices.size() > (unsigned long) maxNumVerts) {
        maxNumVerts = vertices.size();
    }
    if (edges.size() > (unsigned long) maxNumEdges) {
        maxNumEdges = edges.size();
    }


    createLatticeVertexPositions(&Lattice_vert_pos, faces);
    if (orderedNormals.size() == 0) {
        imported = false;
    }
    if (imported) {
        createLatticeVertexNormals(&Lattice_vert_nor, faces, orderedNormals, imported);
        imported = false;
    }
    else {
        createLatticeVertexNormals(&Lattice_vert_nor, faces, orderedNormals, imported);
    }
    createLatticeColors(&Lattice_vert_col, faces);
    createLatticeIndices(&Lattice_idx, faces);

    //    createVertexJointIds(&Lattice_joint_ids, faces);
    //    createVertexJointWeights(&Lattice_joint_weights, faces);

    count = Lattice_idx.size();

    bufIdx.destroy();
    bufPos.destroy();
    bufNor.destroy();
    bufCol.destroy();

    bufIdx.create();
    bufIdx.bind();
    bufIdx.setUsagePattern(QOpenGLBuffer::StaticDraw);
    bufIdx.allocate(Lattice_idx.data(), Lattice_idx.size() * sizeof(GLuint));

    bufPos.create();
    bufPos.bind();
    bufPos.setUsagePattern(QOpenGLBuffer::StaticDraw);
    bufPos.allocate(Lattice_vert_pos.data(), Lattice_vert_pos.size() * sizeof(glm::vec4));

    bufCol.create();
    bufCol.bind();
    bufCol.setUsagePattern(QOpenGLBuffer::StaticDraw);
    bufCol.allocate(Lattice_vert_col.data(), Lattice_vert_col.size() * sizeof(glm::vec4));

    bufNor.create();
    bufNor.bind();
    bufNor.setUsagePattern(QOpenGLBuffer::StaticDraw);
    bufNor.allocate(Lattice_vert_nor.data(), Lattice_vert_nor.size() * sizeof(glm::vec4));

    bufJointId.create();
    bufJointId.bind();
    bufJointId.setUsagePattern(QOpenGLBuffer::StaticDraw);
    bufJointId.allocate(Lattice_joint_ids.data(), Lattice_joint_ids.size() * sizeof(glm::ivec2));

    bufJointWeight.create();
    bufJointWeight.bind();
    bufJointWeight.setUsagePattern(QOpenGLBuffer::StaticDraw);
    bufJointWeight.allocate(Lattice_joint_weights.data(), Lattice_joint_weights.size() * sizeof(glm::vec2));


}

std::vector<QListWidgetItem*> Lattice::getVerts() {
    return vertices;
}

std::vector<QListWidgetItem*> Lattice::getEdges() {
    return edges;
}

std::vector<QListWidgetItem*> Lattice::getFaces() {
    return faces;
}

void Lattice::addVertex(HalfEdge * tgt) {

    Face * f = tgt->getFace();
    if (f->getStartEdge() == tgt) {
        f->setStartEdge(tgt->getNext());
    }
    Face * fs = tgt->getSym()->getFace();
    if (fs->getStartEdge() == tgt->getSym()) {
        fs->setStartEdge(tgt->getSym()->getNext());
    }
    Vertex* start = tgt->getSym()->getVert();
    Vertex* end = tgt->getVert();
    Vertex * newVert = new Vertex();

    glm::vec4 newPos = (start->getPos() + end->getPos());
    newPos /= 2;

    newVert->setPos(newPos);

    // set tgt side edges and pointers
    HalfEdge* split_tgt = new HalfEdge();

    // new edge pointers
    split_tgt->setNext(tgt->getNext());
    split_tgt->setVert(tgt->getVert());
    tgt->getVert()->setEdge(split_tgt);
    split_tgt->setFace(tgt->getFace());
    split_tgt->setSym(tgt->getSym());

    // old edge pointers
    tgt->setNext(split_tgt);
    tgt->setVert(newVert);
    newVert->setEdge(tgt);

    HalfEdge* split_sym = new HalfEdge();

    maxNumEdges++;
    split_tgt->setId(maxNumEdges);
    edges.push_back(split_tgt);
    maxNumEdges++;
    split_sym->setId(maxNumEdges);
    edges.push_back(split_sym);

    maxNumVerts++;
    newVert->setId(maxNumVerts);
    vertices.push_back(newVert);

    split_sym->setNext(tgt->getSym()->getNext());
    split_sym->setVert(tgt->getSym()->getVert());
    split_sym->setFace(tgt->getSym()->getFace());
    split_sym->setSym(tgt);

    tgt->getSym()->getVert()->setEdge(split_sym);

    tgt->getSym()->setNext(split_sym);
    tgt->getSym()->setVert(newVert);

    newVert->setEdge(tgt->getSym());

    tgt->getSym()->setSym(split_tgt);
    tgt->setSym(split_sym);
}

void Lattice::triangulateFace(Face* f) {
    if (f->getVertices().size() != 4) {
        return;
    }
    //    Face* tri1 = new Face();
    Face* tri2 = new Face();

    HalfEdge* tri1_edge = new HalfEdge();
    HalfEdge* tri2_edge = new HalfEdge();

    //    tri1->setColor(f->getColor());
    tri2->setColor(f->getColor());

    tri1_edge->setNext(f->getStartEdge());
    tri1_edge->setVert(f->getStartEdge()->getSym()->getVert());
    tri1_edge->setSym(tri2_edge);
    tri1_edge->setFace(f);
    f->getStartEdge()->getSym()->getVert()->setEdge(tri1_edge);

    tri2_edge->setNext(f->getStartEdge()->getNext()->getNext());
    tri2_edge->setVert(f->getStartEdge()->getNext()->getVert());
    tri2_edge->setSym(tri1_edge);
    tri2_edge->setFace(tri2);
    f->getStartEdge()->getNext()->getVert()->setEdge(tri2_edge);


    //    tri1->setStartEdge(f->getStartEdge());
    tri2->setStartEdge(f->getStartEdge()->getNext()->getNext());

    f->getStartEdge()->getNext()->getNext()->getNext()->setNext(tri2_edge);
    f->getStartEdge()->getNext()->setNext(tri1_edge);

    //    f->getStartEdge()->getNext()->setFace(tri1);
    //    f->getStartEdge()->getNext()->getNext()->setFace(tri1);

    tri2->getStartEdge()->getNext()->setFace(tri2);
    tri2->getStartEdge()->getNext()->getNext()->setFace(tri2);

    maxNumFaces++;
    tri2->setId(maxNumFaces);
    faces.push_back(tri2);

    maxNumEdges++;
    tri1_edge->setId(maxNumEdges);
    maxNumEdges++;
    tri2_edge->setId(maxNumEdges);

    edges.push_back(tri1_edge);
    edges.push_back(tri2_edge);


    updateLattice();

}

void Lattice::deleteVertex(Vertex* v) {
    setWasVertexDeleted(true);
    // get inbound/outbound edges from v
    edgesToLink = {};
    std::vector<Vertex*> vertsToDelete = {};
    std::vector<HalfEdge*> edgesToDelete = {};
    std::vector<Face*> facesToDelete = {};

    vertsToDelete.push_back(v);
    HalfEdge* prevEdge = v->getEdge();
    int numFaces = 0;
    // which faces touch this vertex?


    for (unsigned long i = 0; i < faces.size(); i++) {
        Face* f = (Face*) faces.at(i);
        std::vector<Vertex*> faceVerts = f->getVertices();
        for (unsigned long j = 0; j < faceVerts.size(); j++) {
            // face does touch vertex -> get inbound edge to v from face
            if (v->getId() == faceVerts.at(j)->getId()) {
                numFaces++;
                facesToDelete.push_back(f);
                prevEdge = getInboundEdge(f,v);
                HalfEdge* nextEdge = prevEdge->getNext();
                HalfEdge* linkPrev = prevEdge->getNext();
                while(nextEdge->getId() != prevEdge->getId()) {
                    if (nextEdge->getNext()->getId() == prevEdge->getId()) {
                        linkPrev = nextEdge;
                    }
                    nextEdge=nextEdge->getNext();
                }
                edgesToLink.push_back(linkPrev);
                HalfEdge * linkNext = linkPrev->getNext()->getSym()->getNext();
                edgesToLink.push_back(linkNext);

                // remove prev, prev.sym
                edgesToDelete.push_back(prevEdge);
                edgesToDelete.push_back(prevEdge->getSym());

                nextEdge = prevEdge->getNext();
                //                Vertex* toDelete;
                while (nextEdge->getId() != prevEdge->getId()) {
                    if (!(std::find(faces.begin(), faces.end(), nextEdge->getSym()->getFace()) != faces.end())) {
                        edgesToDelete.push_back(nextEdge);
                        edgesToDelete.push_back(nextEdge->getSym());
                    }
                    nextEdge = nextEdge->getNext();
                }
            }
        }


        for (unsigned long i = 0; i < edges.size(); i++) {
            HalfEdge* delE = (HalfEdge *) edges.at(i);
            if (delE->getVert()->getId() == v->getId()) {
                edgesToDelete.push_back(delE);
                edgesToDelete.push_back(delE->getSym());
            }
        }

    }
    // delete all edges to/from vertex
    for(unsigned long j = 0; j < edgesToDelete.size(); j++) {
        HalfEdge* del = edgesToDelete.at(j);
        for (unsigned long i = 0; i < edges.size(); i++) {
            HalfEdge* delE = (HalfEdge *) edges.at(i);
            //            std::cout << "hereE2" << std::endl;
            if (delE->getId() == del->getId()) {
                delete(del);
                edges.erase(edges.begin()+i);
            }
        }
    }

    for(unsigned long j = 0; j < facesToDelete.size(); j++) {
        Face* del = facesToDelete.at(j);
        for (unsigned long i = 0; i < faces.size(); i++) {
            Face* delE = (Face *) faces.at(i);
            if (delE->getId() == del->getId()) {
                delete(del);
                faces.erase(faces.begin()+i);
            }

        }
    }

    for (unsigned long i = 0; i < vertices.size(); i++) {
        bool hasEdge = false;
        Vertex* v = (Vertex*) vertices.at(i);
        for (unsigned long j = 0; j < edges.size(); j++) {
            HalfEdge* e = (HalfEdge*) edges.at(j);
            if (e->getVert()->getId() == v->getId()) {
                hasEdge = true;
            }
        }
        if (!hasEdge) {
            vertsToDelete.push_back(v);
        }
    }

    for(unsigned long j = 0; j < vertsToDelete.size(); j++) {
        Vertex* del = vertsToDelete.at(j);
        for (unsigned long i = 0; i < vertices.size(); i++) {
            Vertex* delE = (Vertex *) vertices.at(i);
            if (delE->getId() == del->getId()) {
                delete(del);
                vertices.erase(vertices.begin()+i);
            }
        }
    }

    // have edges to link in a vector
    // link vector
    Face* f = new Face();
    if (faces.size() >= 1) {
        maxNumFaces++;
        f->setId(maxNumFaces);

        for (unsigned long i = 0; i < edgesToLink.size() - 1; i+=2) {
            HalfEdge* v = edgesToLink.at(i);
            HalfEdge* n = edgesToLink.at(i+1);
            v->setNext(n);
            v->setFace(f);
            n->setFace(f);
        }
    }

    // delete any extraneous halfedges
    for (unsigned long i = 0; i < edges.size(); i++) {
        HalfEdge* e = (HalfEdge*) edges.at(i);
        if (e->getVert() == NULL) {
            delete(e);
            edges.erase(edges.begin()+i);
        }
        if ((e->getFace() == NULL) && (e->getSym()->getFace() == NULL)) {
            delete(e);
            edges.erase(edges.begin()+i);
        }
    }
}

HalfEdge* Lattice::getInboundEdge(Face* f, Vertex* v) {
    HalfEdge* prevEdge = f->getStartEdge();
    if (prevEdge->getVert()->getId() != v->getId()) {
        HalfEdge* e = prevEdge->getNext();
        while(e != prevEdge) {
            if (e->getVert()->getId() == v->getId()) {
                prevEdge = e;
                break;
            }
            else {
                e = e->getNext();
            }
        }
    }
    return prevEdge;
}

void Lattice::setWasVertexDeleted(bool b) {
    this->vertexWasDeleted = b;
}

bool Lattice::getWasVertexDeleted() {
    return this->vertexWasDeleted;
}

void Lattice::deleteVertFromFace(Face* f, Vertex* v){
    HalfEdge* prevEdge = f->getStartEdge();
    // get prev edge
    if (prevEdge->getVert()->getId() != v->getId()) {
        HalfEdge* e = prevEdge->getNext();
        while(e != prevEdge) {
            if (e->getVert()->getId() == v->getId()) {
                prevEdge = e;
                break;
            }
            else {
                e = e->getNext();
            }
        }
    }
    HalfEdge* nextEdge = prevEdge->getNext();

    HalfEdge* emptyPrev = nextEdge->getNext()->getNext();

    emptyPrev->setNext(prevEdge->getSym()->getNext());
    Face* fNew = new Face();
    emptyPrev->setFace(fNew);

    for (unsigned long i = 0; i < faces.size(); i++) {
        Face* delF = (Face *) faces.at(i);
        if (delF->getId() == f->getId()) {
            faces.erase(faces.begin()+i);
        }
    }

    for (unsigned long i = 0; i < edges.size(); i++) {
        HalfEdge* delE = (HalfEdge *) edges.at(i);
        if (delE->getId() == prevEdge->getId()) {
            delete(prevEdge);
            edges.erase(edges.begin()+i);
        }
        if (delE->getId() == prevEdge->getSym()->getId()) {
            delete(prevEdge->getSym());
            edges.erase(edges.begin()+i);
        }
    }
}

void Lattice::deleteFace(Face* f) {
    for (unsigned long i = 0; i < faces.size(); i++) {
        Face* delF = (Face *) faces.at(i);
        if (delF->getId() == f->getId()) {
            delete(f);
            faces.erase(faces.begin()+i);
        }
    }
}


/// subdivision ///

void Lattice::setOrigFaceVerts(Face* f) {
    std::vector<Vertex*> verts = f->getVertices();
    f->setOriginalVertices(verts);
}

// calculate centroid for each face
Vertex* Lattice::createFaceCentroid(Face* f) {
    // create 4 new faces
    std::vector<Vertex*> verts = f->getVertices();
    f->setOriginalVertices(verts);

    glm::vec4 avgPos = verts.at(0)->getPos();
    for (unsigned long i = 1; i < verts.size(); i++) {
        glm::vec4 vPos = verts.at(i)->getPos();
        avgPos[0] += vPos[0];
        avgPos[1] += vPos[1];
        avgPos[2] += vPos[2];
    }
    avgPos[0] /= verts.size();
    avgPos[1] /= verts.size();
    avgPos[2] /= verts.size();
    Vertex* centroid = new Vertex(avgPos);

    maxNumVerts++;
    centroid->setId(maxNumVerts);
    vertices.push_back(centroid);
    f->setCentroid(centroid);

    return centroid;
}

// bisects each existing edge in the Lattice
void Lattice::splitAllEdges() {
    for (unsigned long i = 0; i < edges.size(); i++) {
        HalfEdge* e = (HalfEdge*) edges.at(i);
        if (e->getWasSubdiv()) {
            if (e->getFace()->getSharp()) {
                e->getVert()->setSharp(true);
            }
            continue;
        }
        else {
            addVertex(e);
            if (e->getFace()->getSharp()) {
                Vertex* v = (Vertex*) vertices.at(vertices.size()-1);
                v->setSharp(true);
            }
            e->setWasSubdiv(true);
            e->getSym()->setWasSubdiv(true);
            e->getNext()->setWasSubdiv(true);
            e->getNext()->getSym()->setWasSubdiv(true);

            if (e->getSharp()) {
                e->getSym()->setSharp(true);
                e->getNext()->setSharp(true);
                e->getNext()->getSym()->setSharp(true);
            }
        }
    }
}

std::vector<HalfEdge*> getLatticeIncidentEdges(Vertex* v ) {
    std::vector<HalfEdge*> edges = {};
    HalfEdge* in = v->getEdge();
    edges.push_back(in);
    HalfEdge* n = in->getNext()->getSym();
    while(n->getId() != in->getId()) {
        edges.push_back(n);
        n = n->getNext()->getSym();
    }
    return edges;
}

std::vector<Face*> getLatticeIncidentFaces(Vertex* v ) {
    std::vector<Face*> faces = {};
    HalfEdge* in = v->getEdge();
    faces.push_back(in->getFace());
    HalfEdge* n = in->getNext()->getSym();
    while(n->getId() != in->getId() && n->getFace()!=NULL) {
        faces.push_back(n->getFace());
        n = n->getNext()->getSym();
    }
    return faces;
}

std::vector<Vertex*> getLatticeAdjacentMidpoints(Vertex* v) {
    std::vector<Vertex*> verts {};
    HalfEdge* in = v->getEdge();
    HalfEdge* n = in->getNext();
    Vertex* c = n->getVert();
    verts.push_back(c);
    while(n->getSym()->getId() != in->getId()) {

        verts.push_back(n->getVert());
        n = n->getSym()->getNext();
    }
    return verts;
}


void Lattice::smoothOriginalVertices(Face *f) {
    for (unsigned long i = 0 ; i < f->getOriginalVertices().size(); i++) {
        Vertex * v = f->getOriginalVertices().at(i);
        if (v->getWasSmoothed() || v->getSharp()) {
            continue;
        }
        else {
            v->setWasSmoothed(true);
            // sum adjacent midpoints

            std::vector<Vertex*> adjacentMidpoints = getLatticeAdjacentMidpoints(v);

            glm::vec4 sumMidpoint = adjacentMidpoints.at(0)->getPos();

            float n = adjacentMidpoints.size();

            for (int j = 1; j < n; j++) {
                sumMidpoint[0] += adjacentMidpoints.at(j)->getPos()[0];
                sumMidpoint[1] += adjacentMidpoints.at(j)->getPos()[1];
                sumMidpoint[2] += adjacentMidpoints.at(j)->getPos()[2];
                //            sumMidpoint[3] += adjacentMidpoints.at(j)->getPos()[3];
            }
            sumMidpoint[0];
            sumMidpoint[1];
            sumMidpoint[2];
            //        sumMidpoint[3] /= (n*n);

            glm::vec4 origPos = v->getPos();
            // sum LatticeIncident centroids

            std::vector<Face*> inFaces = getLatticeIncidentFaces(v);
            glm::vec4 sumCentroid = inFaces.at(0)->getCentroid()->getPos();

            for (unsigned long j = 1; j < inFaces.size(); j++) {
                sumCentroid[0] += inFaces.at(j)->getCentroid()->getPos()[0];
                sumCentroid[1] += inFaces.at(j)->getCentroid()->getPos()[1];
                sumCentroid[2] += inFaces.at(j)->getCentroid()->getPos()[2];
            }
            sumCentroid[0];
            sumCentroid[1];
            sumCentroid[2];

            float s0 = origPos[0]*((n-2)/n) + sumMidpoint[0]/(n*n) + sumCentroid[0]/(n*n);
            float s1 = origPos[1]*((n-2)/n) + sumMidpoint[1]/(n*n) + sumCentroid[1]/(n*n);
            float s2 = origPos[2]*((n-2)/n) + sumMidpoint[2]/(n*n) + sumCentroid[2]/(n*n);

            glm::vec4 smoothedPos = glm::vec4(s0, s1, s2, 1);

            std::vector<HalfEdge*> incidentEdges = getLatticeIncidentEdges(v);
            int sharpEdgeCount = 0;
            for (unsigned long i = 0; i < incidentEdges.size(); i++) {
                HalfEdge* e = (HalfEdge*) incidentEdges.at(i);
                if (e->getSharp()) {
                    sharpEdgeCount++;
                }
            }

            if (sharpEdgeCount > 2) {
                v->setSharp(true);
            }
            else if (sharpEdgeCount == 2) {
                std::vector<glm::vec4> endpoints = {};
                for (unsigned long i = 0; i < incidentEdges.size(); i++) {
                    HalfEdge* e = (HalfEdge*) incidentEdges.at(i);
                    if (e->getSharp()) {
                        Vertex* endpt = e->getSym()->getNext()->getSym()->getNext()->getVert();
                        endpoints.push_back(endpt->getPos());
                    }
                }
                float n0 = origPos[0] * 0.75;
                float n1 = origPos[1] * 0.75;
                float n2 = origPos[2] * 0.75;
                float n3 = origPos[3] * 0.75;

                n0 = n0 + (0.125* endpoints.at(0)[0]) + (0.125* endpoints.at(1)[0]);
                n1 = n1 + (0.125* endpoints.at(0)[1]) + (0.125* endpoints.at(1)[1]);
                n2 = n2 + (0.125* endpoints.at(0)[2]) + (0.125* endpoints.at(1)[2]);
                n3 = n3 + (0.125* endpoints.at(0)[3]) + (0.125* endpoints.at(1)[3]);

                glm::vec4 newPosition = glm::vec4(n0, n1, n2, n3);
                v->setPos(newPosition);
            }
            else {
                v->setPos(smoothedPos);
            }
        }
    }
}


// create center verts and halfedges
void Lattice::quadrangulateFace(Face* f) {

    HalfEdge* start = f->getStartEdge();
    HalfEdge* nextSide = start->getNext()->getNext();
    std::vector<HalfEdge*> newEdges = {};

    // create center halfedges
    while (nextSide->getId() != f->getStartEdge()->getId()) {
        HalfEdge* startSym = start->getSym();

        nextSide = start->getNext()->getNext();
        Vertex* v0 = start->getSym()->getVert(); // vert before split
        Vertex* v1 = start->getVert(); // middle vert
        Vertex* v2 = start->getNext()->getVert(); // vert after split

        // get average vert position
        glm::vec4 avgPos = v0->getPos();
        glm::vec4 vPos = v2->getPos();
        avgPos[0] += vPos[0];
        avgPos[1] += vPos[1];
        avgPos[2] += vPos[2];

        glm::vec4 fCentPos = f->getCentroid()->getPos();

        glm::vec4 fSymCentPos = startSym->getFace()->getCentroid()->getPos();


        avgPos[0] += fCentPos[0] + fSymCentPos[0];
        avgPos[1] += fCentPos[1] + fSymCentPos[1];
        avgPos[2] += fCentPos[2] + fSymCentPos[2];

        avgPos[0] /= 4;
        avgPos[1] /= 4;
        avgPos[2] /= 4;

//        if (!v1->getSharp()) {
//            v1->setPos(avgPos);
//        }

        HalfEdge * centerEdge = new HalfEdge();
        HalfEdge * centerSym = new HalfEdge();

        centerEdge->setSym(centerSym);
        centerSym->setSym(centerEdge);


        maxNumEdges++;
        centerEdge->setId(maxNumEdges);
        edges.push_back(centerEdge);
        maxNumEdges++;
        centerSym->setId(maxNumEdges);
        edges.push_back(centerSym);

        newEdges.push_back(centerSym);
        newEdges.push_back(centerEdge);

        centerEdge->setNext(start->getNext());
        centerEdge->setVert(v1);

        centerSym->setVert(f->getCentroid());
        f->getCentroid()->setEdge(centerSym);
        start->setNext(centerSym);

        // skip to next side of original face
        start = nextSide;
    }

    // link center edges to each other
    for (unsigned long i = 2; i < newEdges.size(); i+=2) {
        newEdges.at(i)->setNext(newEdges.at(i-1));
    }
    newEdges.at(0)->setNext(newEdges.at(newEdges.size()-1));

    // create and link faces
    for (unsigned long i = 0; i < newEdges.size(); i+=2) {
        HalfEdge* se = newEdges.at(i);
        if (se->getFace() == NULL) {
            Face* newFace = new Face();
            newFace->setStartEdge(se);
            newFace->setCentroid(f->getCentroid());
            se->setFace(newFace);
            HalfEdge* ne = se->getNext();
            while (ne->getId() != se->getId()) {
                ne->setFace(newFace);
                ne = ne->getNext();
            }
            maxNumFaces++;
            newFace->setId(maxNumFaces);
//            if (f->getSharp()) {
//                newFace->setSharp(true);
//            }
            faces.push_back(newFace);
        }
    }
}

// returns transformation matrix for Lattice bounding cube
glm::mat4 Lattice::getBoundingBox(int numDivisions) {
    float xmax = std::numeric_limits<float>::min();
    float xmin = std::numeric_limits<float>::max();
    float ymax = std::numeric_limits<float>::min();
    float ymin = std::numeric_limits<float>::max();
    float zmax = std::numeric_limits<float>::min();
    float zmin = std::numeric_limits<float>::max();

    for (unsigned long i = 0; i < vertices.size(); i++) {
        Vertex* v = (Vertex*) vertices.at(i);
        xmax = fmax(v->getPoint_pos()[0], xmax);
        xmin = fmin(v->getPoint_pos()[0], xmin);
        ymax = fmax(v->getPoint_pos()[1], ymax);
        ymin = fmin(v->getPoint_pos()[1], ymin);
        zmax = fmax(v->getPoint_pos()[2], zmax);
        zmin = fmin(v->getPoint_pos()[2], zmin);
    }

    glm::vec3 max_pos = glm::vec3(xmax, ymax, zmax);
    glm::vec3 min_pos = glm::vec3(xmin, ymin, zmin);

    glm::vec3 center = max_pos + min_pos;
    center[0] /= 2;
    center[1] /= 2;
    center[2] /= 2;

    float scaleX = fabs(xmax - xmin);
    float scaleY = fabs(ymax - ymin);
    float scaleZ = fabs(zmax - zmin);

    glm::mat4 trans = glm::translate(glm::mat4(1.0f), center);
    glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(scaleX, scaleY, scaleZ));

    return trans*scale;
}

void Lattice::destroy()
{
    bufIdx.destroy();
    bufPos.destroy();
    bufNor.destroy();
    bufCol.destroy();
}

GLenum Lattice::drawMode()
{
    return GL_LINES;
}

int Lattice::elemCount()
{
    return count;
}

bool Lattice::bindIdx()
{
    return bufIdx.bind();
}

bool Lattice::bindPos()
{
    return bufPos.bind();
}

bool Lattice::bindNor()
{
    return bufNor.bind();
}

bool Lattice::bindCol()
{
    return bufCol.bind();
}

bool Lattice::bindIds()
{
    return bufJointId.bind();
}

bool Lattice::bindWts()
{
    return bufJointWeight.bind();
}

