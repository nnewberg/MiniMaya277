#include "mygl.h"
#include <la.h>

#include <iostream>
#include <QApplication>
#include <QKeyEvent>
#include <ctime>
#include "bmp/EasyBMP.h"

bool divided = false;

Joint *MyGL::getSelectedJoint() const
{
    return selectedJoint;
}

void MyGL::setSelectedJoint(Joint *value)
{
    selectedJoint = value;
}
MyGL::MyGL(QWidget *parent)
    : GLWidget277(parent)
{

    this->geom_mesh = new Mesh();

    setFocusPolicy(Qt::ClickFocus);
    edgeSelect = false;
    faceSelect = false;
    vertSelect = false;
    vertPosUpdate = false;
    splitEdge = false;
    allBindMatrices = {};
    allJointTransformations = {};
    skinned = false;
    drawLattice = false;
    latticeCreated = false;

    ray_o = glm::vec4(0);
    ray_d = glm::vec4(0);

    latticeDivsX = 0;
    latticeDivsY = 0;
    latticeDivsZ = 0;

    allXSlices = {};
    allYSlices = {};
    allZSlices = {};

    closestLatticeVertex = NULL;


}

MyGL::~MyGL()
{
    makeCurrent();
    vao.destroy();
    geom_ray.destroy();
    geom_cylinder.destroy();
    geom_sphere.destroy();
    geom_point.destroy();
    geom_line.destroy();
    geom_lineface.destroy();
    geom_wireSphere.destroy();
    geom_wireBox.destroy();
    geom_lattice.destroy();

}

void MyGL::initializeGL()
{
    // Create an OpenGL context
    initializeOpenGLFunctions();
    // Print out some information about the current OpenGL context
    debugContextVersion();

    // Set a few settings/modes in OpenGL rendering
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_POLYGON_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
    // Set the size with which points should be rendered
    glPointSize(5);
    // Set the color with which the screen is filled at the start of each render call.
    glClearColor(0.5, 0.5, 0.5, 1);

    printGLErrorLog();

    // Create a Vertex Attribute Object
    vao.create();

    // Create the example sphere (you should delete this when you add your own code elsewhere)
    geom_cylinder.create();

    geom_sphere.create();

    geom_mesh->create();

    geom_wireSphere.create();
    geom_wireSphere.setColor(glm::vec4(1,1,1,1));

    geom_wireBox.create();
    geom_lattice.create();

    meshVertices = geom_mesh->getVerts();
    meshEdges = geom_mesh->getEdges();
    meshFaces = geom_mesh->getFaces();

    geom_ray.create();

    latticeVertices = geom_lattice.getVerts();
    latticeEdges = geom_lattice.getEdges();
    latticeFaces = geom_lattice.getFaces();



    for (unsigned long i = 0; i < meshVertices.size(); i++) {
        emit sig_populateVert(meshVertices.at(i));
    }
    for (unsigned long i = 0; i < meshEdges.size(); i++) {
        emit sig_populateEdge(meshEdges.at(i));
    }
    for (unsigned long i = 0; i < meshFaces.size(); i++) {
        emit sig_populateFace(meshFaces.at(i));
    }



    // Create and set up the diffuse shader
    prog_lambert.create(":/glsl/lambert.vert.glsl", ":/glsl/lambert.frag.glsl");
    // Create and set up the wireframe shader
    prog_wire.create(":/glsl/wire.vert.glsl", ":/glsl/wire.frag.glsl");

    prog_joint.create(":/myShader.vert.glsl", ":/myShader.frag.glsl");

    // We have to have a VAO bound in OpenGL 3.2 Core. But if we're not
    // using multiple VAOs, we can just bind one once.
    vao.bind();
}

void MyGL::resizeGL(int w, int h)
{
    camera = Camera(w, h);

    glm::mat4 viewproj = camera.getViewProj();

    // Upload the projection matrix
    prog_lambert.setViewProjMatrix(viewproj);
    prog_wire.setViewProjMatrix(viewproj);

    printGLErrorLog();
}

void MyGL::resetVertexPositions() {
    for (unsigned long i = 0; i < meshVertices.size(); i++) {
        Vertex * v = (Vertex*) meshVertices.at(i);
        v->setPos(v->getDefault_pos());
        if (!skinned) {
            v->setPoint_pos(v->getDefault_pos());
        }
    }
    geom_mesh->updateMesh();
}

// This function is called by Qt any time your GL window is supposed to update
// For example, when the function updateGL is called, paintGL is called implicitly.
void MyGL::paintGL()
{
    //<kerem>
    emit sig_set_meshList();
    //</kerem>
    // Clear the screen so that we only see newly drawn images
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Update the viewproj matrix
    prog_lambert.setViewProjMatrix(camera.getViewProj());
    prog_wire.setViewProjMatrix(camera.getViewProj());
    prog_joint.setViewProjMatrix(camera.getViewProj());

    // draw rays
    if ((ray_o != glm::vec4(0,0,0,0)) && ray_d != glm::vec4(0,0,0,0)) {
        geom_ray.setRay(ray_o,ray_d);
        geom_ray.create();
        prog_wire.setModelMatrix(glm::mat4(1.0f));
        prog_wire.draw(*this, geom_ray);
    }



    // Sphere
    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(-2, 0, 0)) * glm::scale(glm::mat4(1.0f), glm::vec3(3, 3, 3));
    prog_lambert.setModelMatrix(model);
    //    prog_lambert.draw(*this, geom_sphere);
    // Cylinder
    model = glm::translate(glm::mat4(1.0f), glm::vec3(2, 2, 0)) * glm::rotate(glm::mat4(1.0f), -45.0f, glm::vec3(0, 0, 1));
    prog_lambert.setModelMatrix(model);
    //    prog_lambert.draw(*this, geom_cylinder);

    if (vertPosUpdate) {
        //        geom_mesh->destroy();
        geom_mesh->updateMesh();
        vertPosUpdate = false;
    }
    //    if (meshVertices.size() > 0 && allJoints.size() > 0) {

    // draw bounding box
    geom_mesh->getBoundingBox(0);

    if (!skinned) {
        // translates lattice and mesh to have lower corner at origin
        if (!drawLattice) {
            resetVertexPositions();
        }
        prog_lambert.setModelMatrix(glm::mat4(1.0f));
        prog_lambert.draw(*this, *geom_mesh);
    }
    else {
        assignJointTransformations();
        geom_mesh->updateMesh();
        glm::vec3 minPt = geom_mesh->getMin_corner();
        minPt *= -1;
        //        prog_joint.setModelMatrix(glm::translate(glm::mat4(1.0f), minPt));
        prog_joint.setModelMatrix(glm::mat4(1.f));

        prog_joint.draw(*this, *geom_mesh);
    }

    if (allJoints.size() > 0) {
        glDisable( GL_DEPTH_TEST );
        drawJoints(rootJoint, rootJoint->getOverallTransformation());
        glEnable( GL_DEPTH_TEST );

    }

    if (vertSelect) {
        glDisable( GL_DEPTH_TEST );
        glm::vec3 minPt = geom_mesh->getMin_corner();
        minPt *= -1;
        geom_point.create(selectedVertex->getPoint_pos());
        prog_wire.setModelMatrix(glm::mat4(1.0f));
        prog_wire.draw(*this, geom_point);
        glEnable( GL_DEPTH_TEST );
    }
    else if (edgeSelect) {
        glDisable( GL_DEPTH_TEST );
        glm::vec3 minPt = geom_mesh->getMin_corner();
        minPt *= -1;
        glm::vec4 edge_end = selectedEdge->getVert()->getPoint_pos();
        glm::vec4 edge_start = selectedEdge->getSym()->getVert()->getPoint_pos();
        geom_line.create(edge_start, edge_end, false);
        prog_wire.setModelMatrix(glm::mat4(1.0f));
        prog_wire.draw(*this, geom_line);
        glEnable( GL_DEPTH_TEST );
    }
    else if (faceSelect) {
        glDisable( GL_DEPTH_TEST );
        geom_lineface.create(selectedFace);
        prog_wire.setModelMatrix(glm::mat4(1.0f));
        prog_wire.draw(*this, geom_lineface);
        glEnable( GL_DEPTH_TEST );
    }
    if (meshVertices.size() > 0 && drawLattice) {
        latticeRayTraverse();
        for (wirebox* w : latticeCells) {
            prog_wire.setModelMatrix(glm::mat4(1.0f));
            prog_wire.draw(*this, *w);
            emit sig_selectLatticeVertex();
        }
        for (unsigned long i = 0; i < latticeVertices.size(); i++) {
            LatticeVertex* l = (LatticeVertex*) latticeVertices.at(i);
            if (closestLatticeVertex != NULL) {
                if (closestLatticeVertex == l) {

                    prog_wire.setModelMatrix(l->getTransformationMatrix()*glm::scale(glm::mat4(1.0f), glm::vec3(1.3, 1.3, 1.3)));
                    prog_wire.draw(*this, geom_sphere);
                }
            }
            prog_lambert.setModelMatrix(l->getTransformationMatrix());
            prog_lambert.draw(*this, *(l->getSphere()));
        }
    }


}

void MyGL::latticeRayTraverse() {
    float t = -1;
    for (unsigned long i = 0; i < latticeVertices.size(); i++) {
        LatticeVertex* l = (LatticeVertex*) latticeVertices.at(i);
        t = geom_sphere.intersect(geom_ray, l->getTransformationMatrix());
        if (t < tMin && t > 0) {
            tMin = t;
            closestLatticeVertex = l;
        }
    }
}

void MyGL::drawJoints(Joint* root, glm::mat4 T) {
    glm::vec3 minPt = geom_mesh->getMin_corner();
    minPt *= -1;
    glm::mat4 t = T;
    for (QTreeWidgetItem* c : root->getChildren()) {
        Joint* cj = (Joint*) c;
        // draw parent--child line
        geom_line.create(root->getOverallTransformation()*glm::vec4(0,0,0,1),cj->getOverallTransformation()*glm::vec4(0,0,0,1), true);
        prog_wire.setModelMatrix(glm::translate(glm::mat4(1.0f), minPt));
        prog_wire.draw(*this, geom_line);
        drawJoints(cj, root->getOverallTransformation());
    }

    prog_wire.setModelMatrix(root->getOverallTransformation());

    geom_wireSphere.setColor(root->getColor());
    geom_wireSphere.update();
    prog_wire.draw(*this, geom_wireSphere);
}


void MyGL::keyPressEvent(QKeyEvent *e)
{
    // http://doc.qt.io/qt-5/qt.html#Key-enum
    if (e->key() == Qt::Key_Escape) {
        QApplication::quit();
    } else if (e->key() == Qt::Key_Right) {
        camera.theta += 5.0f * DEG2RAD;
    } else if (e->key() == Qt::Key_Left) {
        camera.theta -= 5.0f * DEG2RAD;
    } else if (e->key() == Qt::Key_Up) {
        camera.phi -= 5.0f * DEG2RAD;
    } else if (e->key() == Qt::Key_Down) {
        camera.phi += 5.0f * DEG2RAD;
    } else if (e->key() == Qt::Key_I) {
        camera.zoom -= 0.5f;
    } else if (e->key() == Qt::Key_O) {
        camera.zoom += 0.5f;
    } else if (e->key() == Qt::Key_1) {
        camera.fovy += 5.0f * DEG2RAD;
    } else if (e->key() == Qt::Key_2) {
        camera.fovy -= 5.0f * DEG2RAD;
    }
    else if (e->key() == Qt::Key_L) {
        drawLattice = !drawLattice;
        emit sig_set_lattice_checkbox(drawLattice);

    }
    else if (e->key() == Qt::Key_S) {
        if (edgeSelect) {
            std::cout << "inserting edgeloop" << std::endl;
            geom_mesh->insertEdgeLoop(selectedEdge);
            std::cout << "edgeloop inserted" << std::endl;

            meshVertices = geom_mesh->getVerts();
            meshEdges = geom_mesh->getEdges();
            meshFaces = geom_mesh->getFaces();

            for (unsigned long i = 0; i < meshVertices.size(); i++) {
                emit sig_populateVert(meshVertices.at(i));
            }
            for (unsigned long i = 0; i < meshEdges.size(); i++) {
                emit sig_populateEdge(meshEdges.at(i));
            }
            for (unsigned long i = 0; i < meshFaces.size(); i++) {
                emit sig_populateFace(meshFaces.at(i));
            }
        }

    }
    camera.RecomputeEye();
    update();  // Calls paintGL, among other things
}

void MyGL::selectEdge(HalfEdge* e) {
    this->selectedEdge = e;
    edgeSelect = true;
    faceSelect = false;
    vertSelect = false;
    update();
}

void MyGL::selectVertex(Vertex* v) {
    this->selectedVertex = v;
    edgeSelect = false;
    faceSelect = false;
    vertSelect = true;
    update();
}

void MyGL::selectFace(Face* f) {
    this->selectedFace = f;
    edgeSelect = false;
    faceSelect = true;
    vertSelect = false;
    update();
}

void MyGL::updateVertexPos(glm::vec4 p) {
    selectedVertex->setPos(p);
    update();
}

void MyGL::updateFaceCol(glm::vec4 c) {
    selectedFace->setColor(c);
    geom_mesh->updateMesh();
    update();
}

void MyGL::addVertex() {
    if (edgeSelect) {
        splitEdge = true;
        geom_mesh->addVertex(selectedEdge);
        meshVertices = geom_mesh->getVerts();
        meshEdges = geom_mesh->getEdges();
        meshFaces = geom_mesh->getFaces();

        for (unsigned long i = 0; i < meshVertices.size(); i++) {
            emit sig_populateVert(meshVertices.at(i));
        }
        for (unsigned long i = 0; i < meshEdges.size(); i++) {
            emit sig_populateEdge(meshEdges.at(i));
        }
        for (unsigned long i = 0; i < meshFaces.size(); i++) {
            emit sig_populateFace(meshFaces.at(i));
        }
        update();
    }
}

void MyGL::triangulateFace() {
    if (faceSelect) {
        geom_mesh->triangulateFace(selectedFace);
        meshVertices = geom_mesh->getVerts();
        meshEdges = geom_mesh->getEdges();
        meshFaces = geom_mesh->getFaces();

        for (unsigned long i = 0; i < meshVertices.size(); i++) {
            emit sig_populateVert(meshVertices.at(i));
        }
        for (unsigned long i = 0; i < meshEdges.size(); i++) {
            emit sig_populateEdge(meshEdges.at(i));
        }
        for (unsigned long i = 0; i < meshFaces.size(); i++) {
            emit sig_populateFace(meshFaces.at(i));
        }
        update();
    }
}

void MyGL::deleteVertex() {
    if (vertSelect) {
        geom_mesh->deleteVertex(selectedVertex);
        geom_mesh->updateMesh();
        update();
        vertSelect = false;
    }

}

void MyGL::deleteFace() {
    if (faceSelect) {
        geom_mesh->deleteFace(selectedFace);
        geom_mesh->updateMesh();

        meshVertices = geom_mesh->getVerts();
        meshEdges = geom_mesh->getEdges();
        meshFaces = geom_mesh->getFaces();


        for (unsigned long i = 0; i < meshVertices.size(); i++) {
            emit sig_populateVert(meshVertices.at(i));
        }
        for (unsigned long i = 0; i < meshEdges.size(); i++) {
            emit sig_populateEdge(meshEdges.at(i));
        }
        for (unsigned long i = 0; i < meshFaces.size(); i++) {
            emit sig_populateFace(meshFaces.at(i));
        }
        faceSelect = false;

        update();
    }
}

// subdivide current mesh
void MyGL::createCentroid() {

    if (geom_mesh->getWasVertexDeleted()) {
        std::cout << "Incomplete mesh, will not subdivide" << std::endl;
        return;
    }
    std::cout << std::endl;
    //    clock_t time1 = clock();
    std::cout << "1/4 creating face centroids..." << std::endl;

    for (unsigned long i = 0; i < meshFaces.size(); i++) {
        Face* f = (Face*) meshFaces.at(i);
        geom_mesh->setOrigFaceVerts(f);
        geom_mesh->createFaceCentroid(f);
    }
    std::cout << "1/4 finished" << std::endl;

    std::cout << "2/4 splitting edges..." << std::endl;
    geom_mesh->splitAllEdges();
    std::cout << "2/4 finished" << std::endl;
    meshVertices = geom_mesh->getVerts();


    std::cout << "3/4 quadrangulating faces..." << std::endl;
    for (unsigned long i = 0; i < meshFaces.size(); i++) {
        Face* f = (Face*) meshFaces.at(i);
        geom_mesh->quadrangulateFace(f);
    }
    std::cout << "3/4 finished" << std::endl;
    std::cout << "4/4 smoothing original vertices..." << std::endl;
    for (unsigned long i = 0; i < meshFaces.size(); i++) {
        Face* f = (Face*) meshFaces.at(i);
        geom_mesh->smoothOriginalVertices(f);
    }
    std::cout << "4/4 finished" << std::endl;


    ///    clocks to print subdivision runtim
    //    clock_t time2 = clock();
    //    clock_t timediff = time2 - time1;

    // calculate subdivision time in seconds for funzies
    //    float timediff_sec = ((float)timediff)/CLOCKS_PER_SEC;
    //    std::cout << now << std::endl;
    //    std::cout << "subdiv duration: " << timediff_sec << std::endl;


    meshVertices = geom_mesh->getVerts();
    meshEdges = geom_mesh->getEdges();
    meshFaces = geom_mesh->getFaces();

    // reset edge divide and vertex smooth booleans

    for (unsigned long i = 0; i < meshVertices.size(); i++) {
        Vertex* v = (Vertex*) meshVertices.at(i);
        v->setPoint_pos(v->getPos());
        v->setWasSmoothed(false);
    }


    for (unsigned long i = 0; i < meshEdges.size(); i++) {
        HalfEdge* e = (HalfEdge*) meshEdges.at(i);
        e->setWasSubdiv(false);
    }
    geom_mesh->updateMesh();

    for (unsigned long i = 0; i < meshVertices.size(); i++) {
        emit sig_populateVert(meshVertices.at(i));
    }
    for (unsigned long i = 0; i < meshEdges.size(); i++) {
        emit sig_populateEdge(meshEdges.at(i));
    }
    for (unsigned long i = 0; i < meshFaces.size(); i++) {
        emit sig_populateFace(meshFaces.at(i));
    }
    update();
}

void MyGL::importObj(){
    latticeCreated = false;
    latticeVertices.clear();
    faceSelect = false;
    vertSelect = false;
    edgeSelect = false;
    bool didRead = fileReader.readFile();
    if (!didRead) {
        update();
        return;
    }
    emit sig_clearList();
    geom_mesh->importMesh(fileReader.getVertices(), fileReader.getEdges(), fileReader.getFaces(), fileReader.getOrderedNormals());

    // refresh vertices, faces, and edges in MyGL window
    meshVertices = geom_mesh->getVerts();
    meshEdges = geom_mesh->getEdges();
    meshFaces = geom_mesh->getFaces();
    for (unsigned long i = 0; i < meshVertices.size(); i++) {
        emit sig_populateVert(meshVertices.at(i));
    }
    for (unsigned long i = 0; i < meshEdges.size(); i++) {
        emit sig_populateEdge(meshEdges.at(i));
    }
    for (unsigned long i = 0; i < meshFaces.size(); i++) {
        emit sig_populateFace(meshFaces.at(i));
    }
    update();
}

// sharpen edge/face/vertex
void MyGL::sharpen(int check) {
    if (faceSelect) {
        std::vector<Vertex*> faceVerts = selectedFace->getVertices();
        if (check) {
            selectedFace->setSharp(true);
            for (Vertex * v : faceVerts) {
                v->setSharp(true);
            }
        }
        else {
            selectedFace->setSharp(false);
            for (Vertex * v : faceVerts) {
                v->setSharp(false);
            }
        }
    }
    else if (vertSelect) {
        if (check) {
            selectedVertex->setSharp(true);
        }
        else {
            selectedVertex->setSharp(false);
        }
    }
    else if (edgeSelect) {
        if (check) {
            selectedEdge->setSharp(true);
            selectedEdge->getSym()->setSharp(true);
        }
        else {
            selectedEdge->setSharp(false);
            selectedEdge->getSym()->setSharp(false);
        }
    }
    update();
}

void MyGL::setJointRoot(Joint* r) {
    rootJoint = r;
}

int jIndex = 0;
void MyGL::addAllJoints(Joint* root) {
    root->setId(jIndex);
    jIndex++;
    allJoints.push_back(root); {
        for (unsigned long i = 0; i < root->getChildren().size(); i++) {
            Joint* j = (Joint*) root->getChildren().at(i);
            addAllJoints(j);
        }
    }
}

// import and parse JSON file
void MyGL::importJSON(){
    allJoints = {};
    bool didRead = JSONReader.readJson(rootJoint);
    if (!didRead) {
        update();
        return;
    }
    skinned = false;
    addAllJoints(rootJoint);
    update();
}

bool compareJoint(Joint* j1, Joint* j2) {
    return (j1->getDistToVertex() < j2->getDistToVertex());
}

void MyGL::getNearestJoint(Joint* j, glm::vec4 vPos) {
    glm::vec4 jPos = j->getOverallTransformation()*glm::vec4(0,0,0,1);
    j->setBindMatrix(glm::inverse(j->getOverallTransformation()));
    float dx = (jPos[0] - vPos[0])*(jPos[0] - vPos[0]);
    float dy = (jPos[1] - vPos[1])*(jPos[1] - vPos[1]);
    float dz = (jPos[2] - vPos[2])*(jPos[2] - vPos[2]);

    float dist = sqrtf(dx+dy+dz);
    j->setDistToVertex(dist);
    //    std::cout << "distance " << dist << std::endl;
    vertexJoints.push_back(j);
    for (unsigned long i = 0; i < j->getChildren().size(); i++) {
        Joint* c = (Joint*) j->getChildren().at(i);
        getNearestJoint(c, vPos);
    }
}


void MyGL::assignJointTransformations()
{
    allJointTransformations = {};
    for (unsigned long k = 0; k < allJoints.size(); k++) {
        Joint* j;
        bool found = false;
        for (unsigned long i = 0; i < allJoints.size(); i++) {
            if (allJoints.at(i)->getId() == (int) k) {
                j = allJoints.at(i);
                found = true;
                break;
            }
        }
        if (found) {
            allJointTransformations.push_back(j->getOverallTransformation());
        }
    }
    prog_joint.setJointTransform(allJointTransformations);

    // update vertex position

    for (unsigned long i = 0; i < meshVertices.size(); i++) {
        Vertex * v = (Vertex*) meshVertices.at(i);
        int j0 = v->getInfluenceJoints()[0];
        int j1 = v->getInfluenceJoints()[1];

        float w0 = v->getWeights()[0];
        float w1 = v->getWeights()[1];

        glm::mat4 bindMatrix0 = allBindMatrices[j0];
        glm::mat4 bindMatrix1 = allBindMatrices[j1];

        glm::mat4 jointTrans0 = allJointTransformations[j0];
        glm::mat4 jointTrans1 = allJointTransformations[j1];



        float normW0 = w0/ (w0 + w1);
        float normW1 = w1/ (w0 + w1);

        glm::vec4 p0 = (jointTrans0 * bindMatrix0 * v->getPos())*normW0;
        glm::vec4 p1 = (jointTrans1 * bindMatrix1 * v->getPos())*normW1;

        glm::vec4 new_pos = p0 + p1;
        v->setPoint_pos(new_pos);
    }

    geom_mesh->updateMesh();
}

void MyGL::skin() {
    if ((allJoints.size() <= 0) || (meshVertices.size() <= 0) || (skinned)) {
        std::cout << "no size or already skinned" << std::endl;
        return;
    }
    skinned = true;

    //    addAllJoints(rootJoint);

    //    std::cout << "vert size: " << meshVertices.size() << std::endl;
    for (unsigned long j = 0; j < meshVertices.size(); j++) {
        Vertex* v = (Vertex*) meshVertices.at(j);
        vertexJoints = {};
        //        std::cout << "vert: " << v->getId() << std::endl;
        getNearestJoint(rootJoint, v->getPos());
        // sort joints by distance to current vertex
        std::sort(vertexJoints.begin(), vertexJoints.end(), compareJoint);
        glm::ivec2 vInIds;
        glm::vec2 vInWts;
        for (int i = 0; i < 2; i++) {
            vInIds[i] = (vertexJoints.at(i)->getId());
            vInWts[i] = (vertexJoints.at(i)->getDistToVertex());
        }
        v->setInfluenceJoints(vInIds);
        v->setWeights(vInWts);
    }
    for (unsigned long k = 0; k < allJoints.size(); k++) {
        Joint* j;
        bool found = false;
        for (unsigned long i = 0; i < allJoints.size(); i++) {
            if (allJoints.at(i)->getId() == (int) k) {
                j = allJoints.at(i);
                found = true;
                break;
            }
        }
        if (found) {
            allBindMatrices.push_back(glm::inverse(j->getOverallTransformation()));
            allJointTransformations.push_back(j->getOverallTransformation());
        }
    }
    prog_joint.setBindingMatrix(allBindMatrices);
    prog_joint.setJointTransform(allJointTransformations);
    std::cout << "skin finished" << std::endl;
    geom_mesh->updateMesh();
    update();

}


void MyGL::selectJoint(QTreeWidgetItem * i) {
    for (Joint * j : allJoints) {
        j->setColor(glm::vec4(1,1,1,1));
    }
    selectedJoint = (Joint*) i;
    selectedJoint->setColor(glm::vec4(0,1,1,1));
}

void MyGL::updateJointPosition(glm::vec3 p) {
    selectedJoint->setPosition(p);
    update();
}

void MyGL::updateMesh() {

    geom_mesh->updateMesh();
}

void MyGL::createLatticeCells(float dx, float dy, float dz) {
    glm::mat4 mesh_bb = geom_mesh->getBoundingBox(1);
    latticeDivsX = dx;
    latticeDivsY = dy;
    latticeDivsZ = dz;

    latticeCreated = true;
    latticeCells = {};
    latticeVertices = {};
    float sx = geom_mesh->getBbscale()[0]/dx;
    float sy = geom_mesh->getBbscale()[1]/dy;
    float sz = geom_mesh->getBbscale()[2]/dz;
    glm::vec3 minPt = geom_mesh->getMin_corner();
    glm::mat4 mesh_corner_pos = glm::translate(glm::mat4(1.0f), minPt);
    glm::mat4 corner_pos = glm::translate(glm::mat4(1.0f), glm::vec3(sx/2, sy/2, sz/2));
    glm::mat4 cellScale = glm::scale(glm::mat4(1.0f), glm::vec3(sx, sy, sz));
    for (int i = 0; i <= dx; i++) {
        for (int j = 0; j <= dy; j++) {
            for (int k = 0; k <= dz; k++) {
                // create cell
                //                minPt *= -1;
                glm::mat4 cellTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(i*sx, j*sy, k*sz));
                if (i < dx && j < dy && k < dz) {
                    wirebox* wb = new wirebox();
                    wb->create();
                    for (Vertex* v : wb->getBoxVertices()) {
                        glm::vec4 old_pos = v->getPos();
                        glm::vec4 trans_pos = mesh_corner_pos*corner_pos*cellTranslate*cellScale*old_pos;
                        v->setPos(trans_pos);
                        v->setPoint_pos(trans_pos);
                        //                        std::cout << "v pos: " << trans_pos[0] << " " << trans_pos[1] << " " << trans_pos[2] << std::endl;
                    }
                    wb->update();
                    wb->setTransformationMatrix(mesh_corner_pos*corner_pos*cellTranslate*cellScale);
                    latticeCells.push_back(wb);
                }
                // create lattice vertices
                LatticeVertex * l = new LatticeVertex();
                l->setSphere(&geom_sphere);
                l->setXId(i);
                l->setYId(j);
                l->setZId(k);
                glm::vec4 l_pos = mesh_corner_pos*cellTranslate*glm::vec4(0,0,0,1);
                l->setPosition(l_pos);
                l->setDefaultPosition(l_pos);

                l->setTransformationMatrix(mesh_corner_pos*cellTranslate*glm::scale(glm::mat4(1.0f), glm::vec3(0.1, 0.1, 0.1)));
                l->setDefaultTransformationMatrix(mesh_corner_pos*cellTranslate*glm::scale(glm::mat4(1.0f), glm::vec3(0.1, 0.1, 0.1)));

                latticeVertices.push_back(l);
            }
        }
    }
    assignCellsToLatticeVertices();
    createLatticeSlices(dx, dy, dz);

    //    specialLatticeDeformation(2,0,0);
}

void MyGL::assignCellsToLatticeVertices() {
    for (wirebox* wb : latticeCells) {
        for (Vertex* v : wb->getBoxVertices()) {
            glm::vec4 v_pos = v->getPos();
            for (unsigned long i = 0; i < latticeVertices.size(); i++) {
                LatticeVertex* l = (LatticeVertex*) latticeVertices.at(i);
                if (fabs (v_pos[0] - l->getDefaultPosition()[0]) < 0.01 && fabs (v_pos[1] - l->getDefaultPosition()[1]) < 0.01 && fabs (v_pos[2] - l->getDefaultPosition()[2]) < 0.01) {
                    l->addVertex(v);
                }
            }
        }
    }
}

float factorial( float n )
{
    if (n <= 1) {
        return 1;
    }
    else {
        return n * factorial(n - 1);
    }
}

float bernstein(float i, float n, float s) {
    float nCi = factorial(n)/(factorial(i)*factorial(n-i));
    float sI = pow(s,i) * pow(1-s, n-i);
    return nCi*sI;
}

void MyGL::deformMesh(){
    // iterate through all vertices in mesh and calculate new position according to bernstein wrt all control points
    geom_mesh->getBoundingBox(0);

    glm::vec3 maxPt = geom_mesh->getMax_corner();
    glm::vec3 minPt = geom_mesh->getMin_corner();

    glm::vec3 x0 = glm::vec3(minPt[0], minPt[1], minPt[2]);
    //    glm::vec3 x0 = glm::vec3(mesh_root_pos);
    //    std::cout << "min pt: " << x0[0] << " " << x0[1] << " " << x0[2]  << " " << std::endl;
    //    std::cout << "max pt: " << maxPt[0] << " " << maxPt[1] << " " << maxPt[2]  << " " << std::endl;


    glm::vec3 S3 = glm::vec3(maxPt[0]-minPt[0], 0, 0);
    glm::vec3 T3 = glm::vec3(0, maxPt[1]-minPt[1], 0);
    glm::vec3 U3 = glm::vec3(0, 0, maxPt[2]-minPt[2]);

    //    std::cout << "S: " << S3[0] << " " << S3[1] << " " << S3[2]  << " " << std::endl;
    //    std::cout << "T: " << T3[0] << " " << T3[1] << " " << T3[2]  << " " << std::endl;
    //    std::cout << "U: " << U3[0] << " " << U3[1] << " " << U3[2]  << " " << std::endl;




    for (unsigned long h = 0; h < meshVertices.size(); h++) {
        // get normalized coordinates for each vertex
        Vertex* v = (Vertex*) meshVertices.at(h);
        glm::vec4 old_pos = v->getDefault_pos();

        glm::vec3 vPos = glm::vec3(old_pos[0], old_pos[1], old_pos[2]);

        float s = glm::dot(glm::cross(T3, U3),(vPos - x0))/glm::dot(glm::cross(T3, U3), S3);
        float t = glm::dot(glm::cross(S3, U3),(vPos - x0))/glm::dot(glm::cross(S3, U3), T3);
        float u = glm::dot(glm::cross(S3, T3),(vPos - x0))/glm::dot(glm::cross(S3, T3), U3);

        glm::vec3 deformPos;

        float l = latticeDivsX;
        float m = latticeDivsY;
        float n = latticeDivsZ;
        //        std::cout << l << " " << m << " " << n << std::endl;


        for (int i = 0; i <= l; i++) {
            glm::vec3 jSumPos = glm::vec3(0);

            for (int j = 0; j <= m; j++) {
                glm::vec3 kSumPos = glm::vec3(0);

                for (int k = 0; k <= n; k++) {

                    LatticeVertex* lv = (LatticeVertex*) latticeVertices.at(k + j*(n+1) + i*(m+1)*(n+1));
                    glm::vec3 Pijk = glm::vec3(lv->getPosition()[0], lv->getPosition()[1], lv->getPosition()[2]);
                    //                    std::cout << "Pijk: " << Pijk[0] << " " << Pijk[1] << " " << Pijk[2] << " " << std::endl;

                    glm::vec3 kDeformPos = bernstein(k, n, u)*Pijk;
                    kSumPos += kDeformPos;
                }

                glm::vec3 jDeformPos = bernstein(j, m, t)*kSumPos;
                jSumPos += jDeformPos;
            }
            deformPos += bernstein(i, l, s)*jSumPos;

        }
        glm::vec4 new_pos = glm::vec4(deformPos[0], deformPos[1], deformPos[2], 1);
        //        new_pos[3] = 1;
        //        glm::vec4 new_pos = glm::vec4(fs_coord, ft_coord, fu_coord, 1);
        v->setPoint_pos(new_pos);
        v->setPos(new_pos);
    }
    geom_mesh->updateMesh();
    update();
}

LatticeVertex *MyGL::getClosestLatticeVertex() const
{
    return closestLatticeVertex;
}

void MyGL::setClosestLatticeVertex(LatticeVertex *value)
{
    closestLatticeVertex = value;
}

void::MyGL::createLatticeSlices(int dx, int dy, int dz) {
    // create vectors to store x,y,z "slices" of lattice control points
    // each slice is a vector of latticeVertices
    allXSlices = {};
    for (int i = 0; i <= dx; i++) {
        std::vector<LatticeVertex*> xSlice = {};
        for (unsigned long j = 0; j < latticeVertices.size(); j++) {
            LatticeVertex* l = (LatticeVertex*) latticeVertices.at(j);
            if (l->getXId() == i) {
                xSlice.push_back(l);
            }
        }
        allXSlices.push_back(xSlice);
    }
    allYSlices = {};
    for (int i = 0; i <= dy; i++) {
        std::vector<LatticeVertex*> ySlice = {};
        for (unsigned long j = 0; j < latticeVertices.size(); j++) {
            LatticeVertex* l = (LatticeVertex*) latticeVertices.at(j);
            if (l->getYId() == i) {
                ySlice.push_back(l);
            }
        }
        allYSlices.push_back(ySlice);
    }
    allZSlices = {};
    for (int i = 0; i <= dz; i++) {
        std::vector<LatticeVertex*> zSlice = {};
        for (unsigned long j = 0; j < latticeVertices.size(); j++) {
            LatticeVertex* l = (LatticeVertex*) latticeVertices.at(j);
            if (l->getZId() == i) {
                zSlice.push_back(l);
            }
        }
        allZSlices.push_back(zSlice);
    }
}

void MyGL::resetLattice() {
    for (unsigned long i = 0; i < latticeVertices.size(); i++) {
        LatticeVertex* l = (LatticeVertex*) latticeVertices.at(i);
        l->setTransformationMatrix(l->getDefaultTransformationMatrix());
        for (Vertex* v : l->getLatticeVertices()) {
            v->setPoint_pos(l->getDefaultPosition());
            v->setPos(l->getDefaultPosition());
        }
    }
    for (wirebox* wb : latticeCells) {
        wb->update();
    }
}

void MyGL::specialLatticeDeformation(float amount, int type, int axis) {
    resetLattice();
    resetVertexPositions();
    // type specifies which transformation it is
    amount/=2;
    std::vector<std::vector<LatticeVertex*>> slices;
    if (axis == 0) {
        slices = allXSlices;
    }
    else if (axis == 1) {
        slices = allYSlices;
    }
    else {
        slices = allZSlices;
    }

    // NEW BEND NOT WORKING. WHY
    // bend
    // about x axis
    if (type == 0) {
        if (axis == 0) {
            float k = amount/4;
            float y0 = 0;
            float yMin = -0.5;
            float yMax = 0.5;
            float theta = 0;
            if (k == 0) {
                k = 0.1;
            }
            for (unsigned long i = 0; i < latticeVertices.size(); i++) {
                LatticeVertex* l = (LatticeVertex*) latticeVertices.at(i);
                glm::vec4 new_pos;
                float y = l->getDefaultPosition()[1];
                float yNew;
                float zNew = l->getPosition()[2];
                if (y < yMin) {
                    theta = k*(yMin - y0);
                    yNew = -sin(theta)*(l->getDefaultPosition()[2] - (1/k)) + y0 + cos(theta)*(y-yMin);
                    zNew = cos(theta)*(l->getDefaultPosition()[2] - (1/k)) + (1/k) + sin(theta)*(y-yMin);
                }
                else if (y > yMax) {
                    theta = k*(yMax - y0);
                    yNew = -sin(theta)*(l->getDefaultPosition()[2] - (1/k)) + y0 + cos(theta)*(y-yMax);
                    zNew = cos(theta)*(l->getDefaultPosition()[2] - (1/k)) + (1/k) + sin(theta)*(y-yMax);
                }
                else {
                    theta = k*(y - y0);
                    yNew = -sin(theta)*(l->getDefaultPosition()[2] - (1/k)) + y0;
                    zNew = cos(theta)*(l->getDefaultPosition()[2] - (1/k)) + (1/k);
                }
                l->setTransformationMatrix(glm::translate(glm::mat4(1.f), glm::vec3(l->getDefaultPosition()[0], yNew, zNew))*glm::scale(glm::mat4(1.0f), glm::vec3(0.1, 0.1, 0.1)));
                new_pos = glm::vec4(l->getDefaultPosition()[0], yNew, zNew, 1);

                l->setPosition(new_pos);

                for (Vertex* v : l->getLatticeVertices()) {
                    v->setPoint_pos(new_pos);
                    v->setPos(new_pos);
                }
            }
        }
        else if (axis == 2) {
            float k = amount/4;
            float x0 = 0;
            float xMin = -0.5;
            float xMax = 0.5;
            float theta = 0;
            if (k == 0) {
                k = 0.1;
            }
            for (unsigned long i = 0; i < latticeVertices.size(); i++) {
                LatticeVertex* l = (LatticeVertex*) latticeVertices.at(i);
                glm::vec4 new_pos;
                float x = l->getDefaultPosition()[0];
                float y = l->getDefaultPosition()[1];
                float xNew;
                float zNew;
                if (x < xMin) {
                    theta = k*(xMin - x0);
                    xNew = -sin(theta)*(y - (1/k)) + x0 + cos(theta)*(x-xMin);
                    zNew = cos(theta)*(y - (1/k)) + (1/k) + sin(theta)*(x-xMin);
                }
                else if (x > xMax) {
                    theta = k*(xMax - x0);
                    xNew = -sin(theta)*(y - (1/k)) + x0 + cos(theta)*(x-xMax);
                    zNew = cos(theta)*(y - (1/k)) + (1/k) + sin(theta)*(x-xMax);
                }
                else {
                    theta = k*(x - x0);
                    xNew = -sin(theta)*(y - (1/k)) + x0;
                    zNew = cos(theta)*(y - (1/k)) + (1/k);
                }
                l->setTransformationMatrix(glm::translate(glm::mat4(1.f), glm::vec3(xNew, zNew, l->getDefaultPosition()[2]))*glm::scale(glm::mat4(1.0f), glm::vec3(0.1, 0.1, 0.1)));
                new_pos = glm::vec4(xNew, zNew, l->getDefaultPosition()[2], 1);

                l->setPosition(new_pos);

                for (Vertex* v : l->getLatticeVertices()) {
                    v->setPoint_pos(new_pos);
                    v->setPos(new_pos);
                }
            }
        }
        else if (axis == 1) {
            float k = amount/4;
            float z0 = 0;
            float zMin = -0.5;
            float zMax = 0.5;
            float theta = 0;
            if (k == 0) {
                k = 0.1;
            }
            for (unsigned long i = 0; i < latticeVertices.size(); i++) {
                LatticeVertex* l = (LatticeVertex*) latticeVertices.at(i);
                glm::vec4 new_pos;
                float z = l->getDefaultPosition()[2];
                float x = l->getDefaultPosition()[0];
                float zNew;
                float xNew = l->getPosition()[0];
                if (z < zMin) {
                    theta = k*(zMin - z0);
                    zNew = -sin(theta)*(x - (1/k)) + z0 + cos(theta)*(z-zMin);
                    xNew = cos(theta)*(x - (1/k)) + (1/k) + sin(theta)*(z-zMin);
                }
                else if (z > zMax) {
                    theta = k*(zMax - z0);
                    zNew = -sin(theta)*(x - (1/k)) + z0 + cos(theta)*(z-zMax);
                    xNew = cos(theta)*(x - (1/k)) + (1/k) + sin(theta)*(z-zMax);
                }
                else {
                    theta = k*(z - z0);
                    zNew = -sin(theta)*(x - (1/k)) + z0;
                    xNew = cos(theta)*(x - (1/k)) + (1/k);
                }
                l->setTransformationMatrix(glm::translate(glm::mat4(1.f), glm::vec3(xNew, l->getDefaultPosition()[1], zNew))*glm::scale(glm::mat4(1.0f), glm::vec3(0.1, 0.1, 0.1)));
                new_pos = glm::vec4(xNew, l->getDefaultPosition()[1], zNew, 1);

                l->setPosition(new_pos);

                for (Vertex* v : l->getLatticeVertices()) {
                    v->setPoint_pos(new_pos);
                    v->setPos(new_pos);
                }
            }
        }
    }

    // taper
    if (type == 1) {
        if (axis == 0) {
            float k = amount/4;
            float y0 = 0;
            float yMin = -0.5;
            float yMax = 0.5;
            float theta = 0;
            if (k == 0) {
                k = 0.1;
            }
            for (unsigned long i = 0; i < latticeVertices.size(); i++) {
                LatticeVertex* l = (LatticeVertex*) latticeVertices.at(i);
                glm::vec4 new_pos;
                float y = l->getDefaultPosition()[1];
                float yNew;
                float zNew = l->getPosition()[2];
                if (y < yMin) {
                    theta = k*(yMin - y0);
                    yNew = -sin(theta)*(l->getDefaultPosition()[2] - (1/k)) + y0 + cos(theta)*(y-yMin);
                    zNew = cos(theta)*(l->getDefaultPosition()[2] - (1/k)) + (1/k) + sin(theta)*(y-yMin);
                }
                else if (y > yMax) {
                    theta = k*(yMax - y0);
                    yNew = -sin(theta)*(l->getDefaultPosition()[2] - (1/k)) + y0 + cos(theta)*(y-yMax);
                    zNew = cos(theta)*(l->getDefaultPosition()[2] - (1/k)) + (1/k) + sin(theta)*(y-yMax);
                }
                else {
                    theta = k*(y - y0);
                    yNew = -sin(theta)*(l->getDefaultPosition()[2] - (1/k)) + y0;
                    zNew = cos(theta)*(l->getDefaultPosition()[2] - (1/k)) + (1/k);
                }
                l->setTransformationMatrix(glm::translate(glm::mat4(1.f), glm::vec3(l->getDefaultPosition()[0], yNew, l->getPosition()[2]))*glm::scale(glm::mat4(1.0f), glm::vec3(0.1, 0.1, 0.1)));
                new_pos = glm::vec4(l->getDefaultPosition()[0], yNew, l->getPosition()[2], 1);

                l->setPosition(new_pos);

                for (Vertex* v : l->getLatticeVertices()) {
                    v->setPoint_pos(new_pos);
                    v->setPos(new_pos);
                }
            }
        }
        else if (axis == 2) {
            float k = amount/4;
            float x0 = 0;
            float xMin = -0.5;
            float xMax = 0.5;
            float theta = 0;
            if (k == 0) {
                k = 0.1;
            }
            for (unsigned long i = 0; i < latticeVertices.size(); i++) {
                LatticeVertex* l = (LatticeVertex*) latticeVertices.at(i);
                glm::vec4 new_pos;
                float x = l->getDefaultPosition()[0];
                float y = l->getDefaultPosition()[1];
                float xNew;
                float yNew;
                if (x < xMin) {
                    theta = k*(xMin - x0);
                    xNew = -sin(theta)*(y - (1/k)) + x0 + cos(theta)*(x-xMin);
//                    yNew = cos(theta)*(y - (1/k)) + (1/k) + sin(theta)*(x-xMin);
                }
                else if (x > xMax) {
                    theta = k*(xMax - x0);
                    xNew = -sin(theta)*(y - (1/k)) + x0 + cos(theta)*(x-xMax);
//                    yNew = cos(theta)*(y - (1/k)) + (1/k) + sin(theta)*(x-xMax);
                }
                else {
                    theta = k*(x - x0);
                    xNew = -sin(theta)*(y - (1/k)) + x0;
//                    yNew = cos(theta)*(y - (1/k)) + (1/k);
                }
                l->setTransformationMatrix(glm::translate(glm::mat4(1.f), glm::vec3(xNew, l->getDefaultPosition()[1], l->getDefaultPosition()[2]))*glm::scale(glm::mat4(1.0f), glm::vec3(0.1, 0.1, 0.1)));
                new_pos = glm::vec4(xNew, l->getDefaultPosition()[1], l->getDefaultPosition()[2], 1);

                l->setPosition(new_pos);

                for (Vertex* v : l->getLatticeVertices()) {
                    v->setPoint_pos(new_pos);
                    v->setPos(new_pos);
                }
            }
        }
        else if (axis == 1) {
            float k = amount/4;
            float z0 = 0;
            float zMin = -0.5;
            float zMax = 0.5;
            float theta = 0;
            if (k == 0) {
                k = 0.1;
            }
            for (unsigned long i = 0; i < latticeVertices.size(); i++) {
                LatticeVertex* l = (LatticeVertex*) latticeVertices.at(i);
                glm::vec4 new_pos;
                float z = l->getDefaultPosition()[2];
                float x = l->getDefaultPosition()[0];
                float zNew;
                float xNew = l->getPosition()[0];
                if (z < zMin) {
                    theta = k*(zMin - z0);
                    zNew = -sin(theta)*(x - (1/k)) + z0 + cos(theta)*(z-zMin);
//                    xNew = cos(theta)*(x - (1/k)) + (1/k) + sin(theta)*(z-zMin);
                }
                else if (z > zMax) {
                    theta = k*(zMax - z0);
                    zNew = -sin(theta)*(x - (1/k)) + z0 + cos(theta)*(z-zMax);
//                    xNew = cos(theta)*(x - (1/k)) + (1/k) + sin(theta)*(z-zMax);
                }
                else {
                    theta = k*(z - z0);
                    zNew = -sin(theta)*(x - (1/k)) + z0;
//                    xNew = cos(theta)*(x - (1/k)) + (1/k);
                }
                l->setTransformationMatrix(glm::translate(glm::mat4(1.f), glm::vec3(l->getDefaultPosition()[0], l->getDefaultPosition()[1], zNew))*glm::scale(glm::mat4(1.0f), glm::vec3(0.1, 0.1, 0.1)));
                new_pos = glm::vec4(l->getDefaultPosition()[0], l->getDefaultPosition()[1], zNew, 1);

                l->setPosition(new_pos);

                for (Vertex* v : l->getLatticeVertices()) {
                    v->setPoint_pos(new_pos);
                    v->setPos(new_pos);
                }
            }
        }
    }

    // twist
    else if (type == 2) {
        if (axis == 0) {
            slices = allYSlices;
        }
        else if (axis == 1) {
            slices = allZSlices;

        }
        else if (axis == 2) {
            slices = allXSlices;
        }
        unsigned long numSlices = slices.size();

        for (int i = 0; i < numSlices; i++) {
            for (unsigned long s = 0; s < slices.at(i).size(); s++) {
                LatticeVertex* l = slices.at(i).at(s);
                glm::vec4 old_pos = l->getDefaultPosition();
                glm::vec4 new_pos;
                if (axis == 0) {
                    l->setTransformationMatrix(glm::rotate(glm::mat4(1.f), (i)*amount/numSlices, glm::vec3(0, 1, 0))*l->getDefaultTransformationMatrix());
                    new_pos = l->getTransformationMatrix()*glm::vec4(0,0,0,1);
                }
                else if (axis == 1) {
                    l->setTransformationMatrix(glm::rotate(glm::mat4(1.f), (i)*amount/numSlices, glm::vec3(0, 0, 1))*l->getDefaultTransformationMatrix());
                    new_pos = l->getTransformationMatrix()*glm::vec4(0,0,0,1);
                }
                else if (axis == 2) {
                    l->setTransformationMatrix(glm::rotate(glm::mat4(1.f), (i)*amount/numSlices, glm::vec3(1, 0, 0))*l->getDefaultTransformationMatrix());
                    new_pos = l->getTransformationMatrix()*glm::vec4(0,0,0,1);
                }
                l->setPosition(new_pos);

                for (Vertex* v : l->getLatticeVertices()) {
                    v->setPoint_pos(new_pos);
                    v->setPos(new_pos);
                }
            }
        }
    }

    // squash/stretch
    else if (type == 3) {
        if (axis == 0) {
            slices = allYSlices;
        }
        else if (axis == 1) {
            slices = allZSlices;

        }
        else if (axis == 2) {
            slices = allXSlices;
        }
        unsigned long numSlices = slices.size();
        for (unsigned long i = 0; i < slices.size(); i++) {
            for (unsigned long s = 0; s < slices.at(i).size(); s++) {
                LatticeVertex* l = slices.at(i).at(s);
                glm::vec4 old_pos = l->getDefaultPosition();
                glm::vec4 new_pos;
                if (axis == 0) {
                    new_pos = glm::vec4(old_pos[0] - old_pos[0]*amount/numSlices, old_pos[1] + old_pos[1]*amount/numSlices, old_pos[2] - old_pos[2]*amount/numSlices, old_pos[3]);
                    l->setPosition(new_pos);
                    l->setTransformationMatrix(glm::translate(glm::mat4(1.f), glm::vec3(-old_pos[0]*amount/numSlices, old_pos[1]*amount/numSlices, -old_pos[2]*amount/numSlices))*l->getDefaultTransformationMatrix());
                }
                else if (axis == 1) {
                    new_pos = glm::vec4(old_pos[0] - old_pos[0]*amount/numSlices, old_pos[1] - old_pos[1]*amount/numSlices, old_pos[2] + old_pos[2]*amount/numSlices, old_pos[3]);
                    l->setPosition(new_pos);
                    l->setTransformationMatrix(glm::translate(glm::mat4(1.f), glm::vec3(-old_pos[0]*amount/numSlices, -old_pos[1]*amount/numSlices, old_pos[2]*amount/numSlices))*l->getDefaultTransformationMatrix());
                }
                else if (axis == 2) {
                    new_pos = glm::vec4(old_pos[0] + old_pos[0]*amount/numSlices, old_pos[1] - old_pos[1]*amount/numSlices, old_pos[2] - old_pos[2]*amount/numSlices, old_pos[3]);
                    l->setPosition(new_pos);
                    l->setTransformationMatrix(glm::translate(glm::mat4(1.f), glm::vec3(old_pos[0]*amount/numSlices, -old_pos[1]*amount/numSlices, -old_pos[2]*amount/numSlices))*l->getDefaultTransformationMatrix());
                }
                for (Vertex* v : l->getLatticeVertices()) {
                    v->setPoint_pos(new_pos);
                    v->setPos(new_pos);
                }
            }
        }
    }

    for (wirebox* wb : latticeCells) {
        wb->update();
    }
    update();


}

// raycasting from hw4
void MyGL::mousePressEvent(QMouseEvent * m) {
    tMin = INFINITY;
    selected = false;
    float x = (float) m->x();
    float y = (float) m->y();
    ray n = camera.raycast(x,y);
    ray_o = n.ray_origin;
    ray_d = n.ray_direction;
    update();
}

//<kerem>
void MyGL::slot_raytrace(){
    BMP output;
    output.SetSize(camera.width, camera.height);
    output.SetBitDepth(24);

    for(int x = 0; x < camera.width; x++){
        for(int y = 0; y < camera.height; y++){
            ray r = camera.raycast(x, y);
            glm::vec4 rgb = glm::abs(r.ray_direction) * 255.0f;//r.direction is, of course, a vec4.
            //You'll have to write an absolute value
            //function that takes a vec4 as its argument.

            output(x, y)->Red = rgb[0];
            output(x, y)->Green = rgb[1];
            output(x, y)->Blue = rgb[2];
        }
    }
    output.WriteToFile("rays.bmp");
}
void MyGL::slot_mesh_selected(QListWidgetItem* item) {
    Mesh *m = (Mesh *) item;
    std::cout << "HERE!!!" << std::endl;
    this->geom_mesh = m;
}
bool MyGL::getDrawLattice() const
{
    return drawLattice;
}

void MyGL::setDrawLattice(bool value)
{
    drawLattice = value;
}


//</kerem>

