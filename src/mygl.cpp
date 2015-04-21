#include "mygl.h"
#include <la.h>

#include <iostream>
#include <QApplication>
#include <QKeyEvent>
#include <ctime>



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
    setFocusPolicy(Qt::ClickFocus);
    edgeSelect = false;
    faceSelect = false;
    vertSelect = false;
    vertPosUpdate = false;
    splitEdge = false;
    allBindMatrices = {};
    allJointTransformations = {};
    skinned = false;

}

MyGL::~MyGL()
{
    makeCurrent();
    vao.destroy();
    geom_cylinder.destroy();
    geom_sphere.destroy();
    geom_mesh.destroy();
    geom_point.destroy();
    geom_line.destroy();
    geom_lineface.destroy();
    geom_wireSphere.destroy();
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

    geom_mesh.create();

    geom_wireSphere.create();
    geom_wireSphere.setColor(glm::vec4(1,1,1,1));

    meshVertices = geom_mesh.getVerts();
    meshEdges = geom_mesh.getEdges();
    meshFaces = geom_mesh.getFaces();

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

// This function is called by Qt any time your GL window is supposed to update
// For example, when the function updateGL is called, paintGL is called implicitly.
void MyGL::paintGL()
{
    // Clear the screen so that we only see newly drawn images
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Update the viewproj matrix
    prog_lambert.setViewProjMatrix(camera.getViewProj());
    prog_wire.setViewProjMatrix(camera.getViewProj());
    prog_joint.setViewProjMatrix(camera.getViewProj());


    // Sphere
    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(-2, 0, 0)) * glm::scale(glm::mat4(1.0f), glm::vec3(3, 3, 3));
    prog_lambert.setModelMatrix(model);
    //    prog_lambert.draw(*this, geom_sphere);
    // Cylinder
    model = glm::translate(glm::mat4(1.0f), glm::vec3(2, 2, 0)) * glm::rotate(glm::mat4(1.0f), -45.0f, glm::vec3(0, 0, 1));
    prog_lambert.setModelMatrix(model);
    //    prog_lambert.draw(*this, geom_cylinder);

    if (vertPosUpdate) {
        //        geom_mesh.destroy();
        geom_mesh.updateMesh();
        vertPosUpdate = false;
    }
    //    if (meshVertices.size() > 0 && allJoints.size() > 0) {



    if (!skinned) {
        prog_lambert.setModelMatrix(glm::mat4(1.0f));
        prog_lambert.draw(*this, geom_mesh);
    }
    else {
        assignJointTransformations();
        geom_mesh.updateMesh();
        prog_joint.setModelMatrix(glm::mat4(1.0f));
        prog_joint.draw(*this, geom_mesh);
    }

    if (allJoints.size() > 0) {
        glDisable( GL_DEPTH_TEST );
        drawJoints(rootJoint, rootJoint->getOverallTransformation());
        glEnable( GL_DEPTH_TEST );

    }

    if (vertSelect) {
        glDisable( GL_DEPTH_TEST );
        geom_point.create(selectedVertex->getPoint_pos());
        prog_wire.setModelMatrix(glm::mat4(1.0f));
        prog_wire.draw(*this, geom_point);
        glEnable( GL_DEPTH_TEST );
    }
    else if (edgeSelect) {
        glDisable( GL_DEPTH_TEST );
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
}

void MyGL::drawJoints(Joint* root, glm::mat4 T) {
    glm::mat4 t = T;
    for (QTreeWidgetItem* c : root->getChildren()) {
        Joint* cj = (Joint*) c;
        // draw parent--child line
        geom_line.create(root->getOverallTransformation()*glm::vec4(0,0,0,1),cj->getOverallTransformation()*glm::vec4(0,0,0,1), true);
        prog_wire.setModelMatrix(glm::mat4(1.0f));
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
    geom_mesh.updateMesh();
    update();
}

void MyGL::addVertex() {
    if (edgeSelect) {
        splitEdge = true;
        geom_mesh.addVertex(selectedEdge);
        meshVertices = geom_mesh.getVerts();
        meshEdges = geom_mesh.getEdges();
        meshFaces = geom_mesh.getFaces();

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
        geom_mesh.triangulateFace(selectedFace);
        meshVertices = geom_mesh.getVerts();
        meshEdges = geom_mesh.getEdges();
        meshFaces = geom_mesh.getFaces();

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
        geom_mesh.deleteVertex(selectedVertex);
        geom_mesh.updateMesh();
        update();
        vertSelect = false;
    }

}

void MyGL::deleteFace() {
    if (faceSelect) {
        geom_mesh.deleteFace(selectedFace);
        geom_mesh.updateMesh();

        meshVertices = geom_mesh.getVerts();
        meshEdges = geom_mesh.getEdges();
        meshFaces = geom_mesh.getFaces();


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

    if (geom_mesh.getWasVertexDeleted()) {
        std::cout << "Incomplete mesh, will not subdivide" << std::endl;
        return;
    }
    std::cout << std::endl;
    //    clock_t time1 = clock();
    std::cout << "1/4 creating face centroids..." << std::endl;

    for (unsigned long i = 0; i < meshFaces.size(); i++) {
        Face* f = (Face*) meshFaces.at(i);
        geom_mesh.setOrigFaceVerts(f);
        geom_mesh.createFaceCentroid(f);
    }
    std::cout << "1/4 finished" << std::endl;

    std::cout << "2/4 splitting edges..." << std::endl;
    geom_mesh.splitAllEdges();
    std::cout << "2/4 finished" << std::endl;
    meshVertices = geom_mesh.getVerts();


    std::cout << "3/4 quadrangulating faces..." << std::endl;
    for (unsigned long i = 0; i < meshFaces.size(); i++) {
        Face* f = (Face*) meshFaces.at(i);
        geom_mesh.quadrangulateFace(f);
    }
    std::cout << "3/4 finished" << std::endl;
    std::cout << "4/4 smoothing original vertices..." << std::endl;
    for (unsigned long i = 0; i < meshFaces.size(); i++) {
        Face* f = (Face*) meshFaces.at(i);
        geom_mesh.smoothOriginalVertices(f);
    }
    std::cout << "4/4 finished" << std::endl;


    ///    clocks to print subdivision runtim
    //    clock_t time2 = clock();
    //    clock_t timediff = time2 - time1;

    // calculate subdivision time in seconds for funzies
    //    float timediff_sec = ((float)timediff)/CLOCKS_PER_SEC;
    //    std::cout << now << std::endl;
    //    std::cout << "subdiv duration: " << timediff_sec << std::endl;


    meshVertices = geom_mesh.getVerts();
    meshEdges = geom_mesh.getEdges();
    meshFaces = geom_mesh.getFaces();

    // reset edge divide and vertex smooth booleans

    for (unsigned long i = 0; i < meshVertices.size(); i++) {
        Vertex* v = (Vertex*) meshVertices.at(i);
        v->setWasSmoothed(false);
    }


    for (unsigned long i = 0; i < meshEdges.size(); i++) {
        HalfEdge* e = (HalfEdge*) meshEdges.at(i);
        e->setWasSubdiv(false);
    }
    geom_mesh.updateMesh();

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
    faceSelect = false;
    vertSelect = false;
    edgeSelect = false;
    bool didRead = fileReader.readFile();
    if (!didRead) {
        update();
        return;
    }
    emit sig_clearList();
    geom_mesh.importMesh(fileReader.getVertices(), fileReader.getEdges(), fileReader.getFaces(), fileReader.getOrderedNormals());

    // refresh vertices, faces, and edges in MyGL window
    meshVertices = geom_mesh.getVerts();
    meshEdges = geom_mesh.getEdges();
    meshFaces = geom_mesh.getFaces();
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

    //    Joint* c1 = (Joint* ) root->getChildren().at(0);
    //    QString rootname = c1->getName();
    //    std::cout << "root: " << root->getName().toUtf8().constData() << std::endl;
    //    std::cout << root->getPosition()[0] << " " << root->getPosition()[1] << " " << root->getPosition()[2] << std::endl;
    //    std::cout << root->getChildren().size() << std::endl;
    //    Joint* pr = (Joint*) root->getParent();


    //    std::cout << "c1: " << rootname.toUtf8().constData() << std::endl;
    //    std::cout << c1->getPosition()[0] << " " << c1->getPosition()[1] << " " << c1->getPosition()[2] << std::endl;
    //    std::cout << c1->getChildren().size() << std::endl;
    //    Joint* p = (Joint*) c1->getParent();
    //    std::cout << p->getName().toUtf8().constData() << std::endl;
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
        for (int i = 0; i < allJoints.size(); i++) {
            if (allJoints.at(i)->getId() == k) {
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


    geom_mesh.updateMesh();
}

void MyGL::skin() {
    if ((allJoints.size() <= 0) || (meshVertices.size() <= 0) || (skinned)) {
        std::cout << "no size or already skinned" << std::endl;
        return;
    }
    skinned = true;

//    addAllJoints(rootJoint);

//    std::cout << "vert size: " << meshVertices.size() << std::endl;
    for (int j = 0; j < meshVertices.size(); j++) {
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

//        std::cout << v->getId() << " ids " << v->getInfluenceJoints()[0] << " " << v->getInfluenceJoints()[1] << std::endl;
//        std::cout << v->getId() << " wts " << v->getWeights()[0] << " " << v->getWeights()[1] << std::endl;

    }
    for (unsigned long k = 0; k < allJoints.size(); k++) {
//        std::cout << "id: " << allJoints.at(k)->getId() << std::endl;
        Joint* j;
        bool found = false;
        for (int i = 0; i < allJoints.size(); i++) {
            if (allJoints.at(i)->getId() == k) {
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
    geom_mesh.updateMesh();
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

    geom_mesh.updateMesh();
}
