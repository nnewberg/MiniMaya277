#include "mainwindow.h"
#include <ui_mainwindow.h>
#include <iostream>
#include "mygl.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->mygl->setFocusPolicy(Qt::ClickFocus);

    ui->doubleSpinBox->setRange(-20.0, 20.0);
    ui->doubleSpinBox_2->setRange(-20.0, 20.0);
    ui->doubleSpinBox_3->setRange(-20.0, 20.0);

    ui->doubleSpinBox->setSingleStep(0.1);
    ui->doubleSpinBox_2->setSingleStep(0.1);
    ui->doubleSpinBox_3->setSingleStep(0.1);

    ui->doubleSpinBox_4->setRange(0.0, 1.0);
    ui->doubleSpinBox_5->setRange(0.0, 1.0);
    ui->doubleSpinBox_6->setRange(0.0, 1.0);

    ui->doubleSpinBox_4->setSingleStep(0.1);
    ui->doubleSpinBox_5->setSingleStep(0.1);
    ui->doubleSpinBox_6->setSingleStep(0.1);


    ui->doubleSpinBox_10->setRange(-20.0, 20.0);
    ui->doubleSpinBox_11->setRange(-20.0, 20.0);
    ui->doubleSpinBox_12->setRange(-20.0, 20.0);

    ui->doubleSpinBox_10->setSingleStep(0.1);
    ui->doubleSpinBox_11->setSingleStep(0.1);
    ui->doubleSpinBox_12->setSingleStep(0.1);

    ui->doubleSpinBox_9->setSingleStep(5.0);
    ui->doubleSpinBox_7->setSingleStep(5.0);
    ui->doubleSpinBox_8->setSingleStep(5.0);

    ui->doubleSpinBox_9->setRange(-360.0, 360.0);
    ui->doubleSpinBox_7->setRange(-360.0, 360.0);
    ui->doubleSpinBox_8->setRange(-360.0, 360.0);

    ui->latticeX_spinbox->setRange(-100, 100);
    ui->latticeY_spinbox->setRange(-100, 100);
    ui->latticeZ_spinbox->setRange(-100, 100);

    ui->latticeX_spinbox->setSingleStep(0.1);
    ui->latticeY_spinbox->setSingleStep(0.1);
    ui->latticeZ_spinbox->setSingleStep(0.1);

    ui->deform_slider->setRange(-10,10);

    changeable = false;

    ui->checkBox->setChecked(false);

    currentVertex = NULL;
    currentFace = NULL;
    currentJoint = NULL;
    currentFrame = nullptr;

    Joint * r = new Joint();
    r->setName("root");
    r->setText(0, "root");
    ui->mygl->setJointRoot(r);
    ui->treeWidget->addTopLevelItem(r);
    ui->mygl->update();

    deformType = 0;
    deformAxis = 0;

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionQuit_triggered()
{
    QApplication::exit();
}

void MainWindow::slot_populateVert(QListWidgetItem* i) {
    ui->vertex_listWidget->insertItem(0, i);
}


void MainWindow::on_edge_listWidget_itemClicked(QListWidgetItem *item)
{
    HalfEdge* e = (HalfEdge*) item;
    ui->mygl->selectEdge(e);
    bool isSharp = e->getSharp();
    ui->checkBox->setChecked(isSharp);
    currentVertex = NULL;
}

void MainWindow::on_vertex_listWidget_itemClicked(QListWidgetItem *item)
{
    Vertex* v = (Vertex*) item;
    currentVertex = v;
    ui->mygl->selectVertex(v);
    ui->doubleSpinBox->setValue(v->getPos()[0]);
    ui->doubleSpinBox_2->setValue(v->getPos()[1]);
    ui->doubleSpinBox_3->setValue(v->getPos()[2]);
    bool isSharp = v->getSharp();
    ui->checkBox->setChecked(isSharp);
}

void MainWindow::on_face_listWidget_itemClicked(QListWidgetItem *item)
{
    Face* f = (Face*) item;
    currentFace = f;
    ui->mygl->selectFace(f);
    ui->doubleSpinBox_4->setValue(f->getColor()[0]);
    ui->doubleSpinBox_5->setValue(f->getColor()[1]);
    ui->doubleSpinBox_6->setValue(f->getColor()[2]);
    bool isSharp = f->getSharp();
    ui->checkBox->setChecked(isSharp);
}

void MainWindow::on_mygl_sig_populateVert(QListWidgetItem * item)
{
    ui->vertex_listWidget->addItem(item);
}

void MainWindow::on_mygl_sig_populateFace(QListWidgetItem * item)
{
    ui->face_listWidget->addItem(item);
}

void MainWindow::on_mygl_sig_populateEdge(QListWidgetItem * item)
{
    ui->edge_listWidget->addItem(item);
}

void MainWindow::on_doubleSpinBox_valueChanged(double arg1)
{
    if (currentVertex != NULL) {
        glm::vec4 old_pos = currentVertex->getPos();
        if (arg1 != old_pos[0]) {
            glm::vec4 new_pos = glm::vec4(arg1, old_pos[1], old_pos[2], 1);
            currentVertex->setPos(new_pos);
            ui->mygl->vertPosUpdate = true;
            ui->mygl->updateVertexPos(new_pos);
            ui->mygl->update();
        }
    }
}

void MainWindow::on_doubleSpinBox_2_valueChanged(double arg1)
{
    if (currentVertex != NULL) {
        glm::vec4 old_pos = currentVertex->getPos();
        if (arg1 != old_pos[1]) {
            glm::vec4 new_pos = glm::vec4(old_pos[0], arg1, old_pos[2], 1);
            currentVertex->setPos(new_pos);
            ui->mygl->vertPosUpdate = true;
            ui->mygl->updateVertexPos(new_pos);
            ui->mygl->update();
        }
    }
}

void MainWindow::on_doubleSpinBox_3_valueChanged(double arg1)
{
    if (currentVertex != NULL) {
        glm::vec4 old_pos = currentVertex->getPos();
        if (arg1 != old_pos[2]) {
            glm::vec4 new_pos = glm::vec4(old_pos[0], old_pos[1], arg1, 1);
            currentVertex->setPos(new_pos);
            ui->mygl->vertPosUpdate = true;
            ui->mygl->updateVertexPos(new_pos);
            ui->mygl->update();
        }
    }
}

void MainWindow::on_pushButton_clicked()
{
    ui->mygl->addVertex();
}

void MainWindow::on_pushButton_2_clicked()
{
    ui->mygl->triangulateFace();
}

void MainWindow::on_doubleSpinBox_4_valueChanged(double arg1)
{
    if (currentFace != NULL) {
        glm::vec4 old_col = currentFace->getColor();
        if (arg1 != old_col[0]) {
            glm::vec4 new_col = glm::vec4(arg1, old_col[1], old_col[2], 1);
            currentFace->setColor(new_col);
            ui->mygl->updateFaceCol(new_col);
            ui->mygl->update();
        }
    }
}

void MainWindow::on_doubleSpinBox_5_valueChanged(double arg1)
{
    if (currentFace != NULL) {
        glm::vec4 old_col = currentFace->getColor();
        if (arg1 != old_col[1]) {
            glm::vec4 new_col = glm::vec4(old_col[0], arg1,  old_col[2], 1);
            currentFace->setColor(new_col);
            ui->mygl->updateFaceCol(new_col);
            ui->mygl->update();
        }
    }
}

void MainWindow::on_doubleSpinBox_6_valueChanged(double arg1)
{
    if (currentFace != NULL) {
        glm::vec4 old_col = currentFace->getColor();
        if (arg1 != old_col[2]) {
            glm::vec4 new_col = glm::vec4(old_col[0], old_col[1], arg1, 1);
            currentFace->setColor(new_col);
            ui->mygl->updateFaceCol(new_col);
            ui->mygl->update();
        }
    }
}

void MainWindow::on_pushButton_3_clicked()
{
    ui->mygl->deleteVertex();
}

void MainWindow::on_pushButton_4_clicked()
{
    ui->mygl->createCentroid();
}

void MainWindow::on_pushButton_5_clicked()
{
    ui->vertex_listWidget->clearSelection();
    ui->face_listWidget->clearSelection();
    ui->edge_listWidget->clearSelection();
    ui->mygl->importObj();
    ui->mygl->update();
    ui->checkBox->setChecked(false);

}

void MainWindow::on_checkBox_stateChanged(int arg1)
{
    ui->mygl->sharpen(arg1);
}

void MainWindow::on_pushButton_6_clicked()
{
    ui->mygl->importJSON();
}

void MainWindow::slot_addTreeRoot(QTreeWidgetItem * jRoot) {
    ui->treeWidget->addTopLevelItem(jRoot);
}

void MainWindow::on_treeWidget_itemClicked(QTreeWidgetItem *item, int column)
{
    if (item != NULL) {
        Joint* j = (Joint*) item;
        currentJoint = j;
        //set currentJoint in timeline
        ui->timeline_listWidget->setSelectedJoint(currentJoint);
        ui->mygl->selectJoint(item);
        ui->doubleSpinBox_11->setValue(j->getPosition()[0]);
        ui->doubleSpinBox_10->setValue(j->getPosition()[1]);
        ui->doubleSpinBox_12->setValue(j->getPosition()[2]);

        glm::quat rotation = j->getRotation();
        glm::vec3 angles = glm::eulerAngles(rotation);

        changeable = false;
        ui->doubleSpinBox_9->setValue(angles[0]*180/PI);
        ui->doubleSpinBox_7->setValue(angles[1]*180/PI);
        ui->doubleSpinBox_8->setValue(angles[2]*180/PI);
        changeable = true;

        rx = angles[0]*180/PI;
        ry = angles[1]*180/PI;
        rz = angles[2]*180/PI;
    }

}

void MainWindow::on_treeWidget_itemSelectionChanged()
{
    if (ui->treeWidget->currentItem() != NULL) {
        ui->mygl->selectJoint(ui->treeWidget->currentItem());
        currentJoint = (Joint*) ui->treeWidget->currentItem();
    }
}

// joint position
void MainWindow::on_doubleSpinBox_11_valueChanged(double arg1)
{
    if (currentJoint != NULL) {
        glm::vec3 old_pos = currentJoint->getPosition();
        if (arg1 != old_pos[0]) {
            glm::vec3 new_pos = glm::vec3(arg1, old_pos[1], old_pos[2]);
            currentJoint->setPosition(new_pos);
            if (currentFrame){
                currentFrame->setJointPos(currentJoint, new_pos);
            }
            ui->mygl->updateJointPosition(new_pos);
            ui->mygl->updateMesh();
            ui->mygl->update();
        }
    }
}

void MainWindow::on_doubleSpinBox_10_valueChanged(double arg1)
{
    if (currentJoint != NULL) {
        glm::vec3 old_pos = currentJoint->getPosition();
        if (arg1 != old_pos[1]) {
            glm::vec3 new_pos = glm::vec3(old_pos[0], arg1, old_pos[2]);
            currentJoint->setPosition(new_pos);
            if (currentFrame){
                currentFrame->setJointPos(currentJoint, new_pos);
            }
            ui->mygl->updateJointPosition(new_pos);
            ui->mygl->updateMesh();
            ui->mygl->update();
        }
    }
}

void MainWindow::on_doubleSpinBox_12_valueChanged(double arg1)
{
    if (currentJoint != NULL) {
        glm::vec3 old_pos = currentJoint->getPosition();
        if (arg1 != old_pos[2]) {
            glm::vec3 new_pos = glm::vec3(old_pos[0], old_pos[1], arg1);
            currentJoint->setPosition(new_pos);
            if (currentFrame){
                currentFrame->setJointPos(currentJoint, new_pos);
            }
            ui->mygl->updateJointPosition(new_pos);
            ui->mygl->updateMesh();
            ui->mygl->update();
        }
    }
}

// joint rotation
void MainWindow::on_doubleSpinBox_9_valueChanged(double arg1)
{
    if (currentJoint != NULL && changeable) {
        glm::quat old_rotation = currentJoint->getRotation();
        float sign;
        if (arg1 < rx) {
            sign = -5;
        }
        if (arg1 > rx) {
            sign = 5;
        }
        float angle = sign*PI/180;
        rx = arg1;
        glm::quat new_rotation = glm::rotate(glm::quat(), angle, glm::vec3(1,0,0))*old_rotation;
        currentJoint->setRotation(glm::normalize(new_rotation));
        if (currentFrame){
            currentFrame->setJointRot(currentJoint, new_rotation);
        }
//        ui->mygl->updateMesh();
        ui->mygl->update();
    }

}

void MainWindow::on_doubleSpinBox_7_valueChanged(double arg1)
{
    if (currentJoint != NULL && changeable) {
        glm::quat old_rotation = currentJoint->getRotation();
        float sign;
        if (arg1 < ry) {
            sign = -5;
        }
        if (arg1 > ry) {
            sign = 5;
        }
        float angle = sign*PI/180;
        ry = arg1;
        glm::quat new_rotation = glm::rotate(glm::quat(), angle, glm::vec3(0,1,0))*old_rotation;
        currentJoint->setRotation(glm::normalize(new_rotation));
        if (currentFrame){
            currentFrame->setJointRot(currentJoint, new_rotation);
        }
//        ui->mygl->updateMesh();
        ui->mygl->update();
    }
}

void MainWindow::on_doubleSpinBox_8_valueChanged(double arg1)
{
    if (currentJoint != NULL && changeable) {
        glm::quat old_rotation = currentJoint->getRotation();
        float sign;
        if (arg1 < rz) {
            sign = -5;
        }
        if (arg1 > rz) {
            sign = 5;
        }
        float angle = sign*PI/180;
        glm::quat new_rotation = glm::rotate(glm::quat(), angle, glm::vec3(0,0,1))*old_rotation;
        rz = arg1;
        currentJoint->setRotation(glm::normalize(new_rotation));
        if (currentFrame){
            currentFrame->setJointRot(currentJoint, new_rotation);
        }
//        ui->mygl->updateMesh();
        ui->mygl->update();
    }
}
Joint *MainWindow::getCurrentJoint() const
{
    return currentJoint;
}

void MainWindow::setCurrentJoint(Joint *value)
{
    currentJoint = value;
}


void MainWindow::on_pushButton_7_clicked()
{
    ui->mygl->skin();
    ui->mygl->update();
}


void MainWindow::on_timeline_listWidget_itemPressed(QListWidgetItem *item)
{
    if (currentJoint){
        Frame* frame = ((Frame*)item);
        currentFrame = frame;
        if (frame->animatesJoint(currentJoint)){
            std::cout<<"Found Data on This Frame for This Joint"<<std::endl;
            glm::vec3 frame_pos = frame->getJointPos(currentJoint);
            std::cout<<"Pos: "<<frame_pos[0]<<" "<< frame_pos[1]<<" "<<frame_pos[2]<<std::endl;
            glm::fquat frame_rot = frame->getJointRot(currentJoint);
            std::cout<<"Rot: "<<frame_rot[0]<<" "<< frame_rot[1]<<" "<<frame_rot[2]<<std::endl;
            currentJoint->setPosition(frame_pos);
            currentJoint->setRotation(frame_rot);
            ui->mygl->updateJointPosition(frame_pos);
        }else{
            std::cout<<"Data Not Found on Frame for this Joint"<<std::endl;

            frame->setJointPosRot(currentJoint, currentJoint->getPosition(), currentJoint->getRotation());
        }

        ui->doubleSpinBox_11->setValue(currentJoint->getPosition()[0]);
        ui->doubleSpinBox_10->setValue(currentJoint->getPosition()[1]);
        ui->doubleSpinBox_12->setValue(currentJoint->getPosition()[2]);

        glm::quat rotation = currentJoint->getRotation();
        glm::vec3 angles = glm::eulerAngles(rotation);
        changeable = false;
        ui->doubleSpinBox_9->setValue(angles[0]*180/PI);
        ui->doubleSpinBox_7->setValue(angles[1]*180/PI);
        ui->doubleSpinBox_8->setValue(angles[2]*180/PI);
        changeable = true;
    }

}


// lattice manipulation
void MainWindow::on_latticeX_spinbox_valueChanged(double arg1)
{
    if (changeable) {
        if (ui->mygl->getClosestLatticeVertex() != NULL) {
            LatticeVertex * l = ui->mygl->getClosestLatticeVertex();
            glm::vec4 l_pos = l->getPosition();
            glm::vec4 new_l_pos4 = glm::vec4(arg1, l_pos[1], l_pos[2], l_pos[3]);
            glm::vec3 new_l_pos3 = glm::vec3 (arg1, l_pos[1], l_pos[2]);
            glm::mat4 lTransform = glm::translate(glm::mat4(1.0f), new_l_pos3)*glm::scale(glm::mat4(1.0f), glm::vec3(0.1, 0.1, 0.1));
            l->setPosition(new_l_pos4);
            l->setTransformationMatrix(lTransform);
            for (Vertex* v : ui->mygl->getClosestLatticeVertex()->getLatticeVertices()) {
                v->setPoint_pos(new_l_pos4);
                v->setPos(new_l_pos4);
            }
        }
        for (wirebox* wb : ui->mygl->latticeCells) {
            wb->update();
        }
        ui->mygl->deformMesh();
        ui->mygl->update();
    }
}

void MainWindow::on_latticeY_spinbox_valueChanged(double arg1)
{
    if (changeable) {
        if (ui->mygl->getClosestLatticeVertex() != NULL) {
            LatticeVertex * l = ui->mygl->getClosestLatticeVertex();
            glm::vec4 l_pos = l->getPosition();
            glm::vec4 new_l_pos4 = glm::vec4(l_pos[0], arg1, l_pos[2], l_pos[3]);
            glm::vec3 new_l_pos3 = glm::vec3 (l_pos[0], arg1, l_pos[2]);
            glm::mat4 lTransform = glm::translate(glm::mat4(1.0f), new_l_pos3)*glm::scale(glm::mat4(1.0f), glm::vec3(0.1, 0.1, 0.1));
            l->setPosition(new_l_pos4);
            l->setTransformationMatrix(lTransform);

            std::cout << ui->mygl->getClosestLatticeVertex()->getLatticeVertices().size() << std::endl;
            for (Vertex* v : ui->mygl->getClosestLatticeVertex()->getLatticeVertices()) {
                v->setPoint_pos(new_l_pos4);
                v->setPos(new_l_pos4);
            }
        }
        for (wirebox* wb : ui->mygl->latticeCells) {
            wb->update();
        }
        ui->mygl->deformMesh();
        ui->mygl->update();
    }
}

void MainWindow::on_latticeZ_spinbox_valueChanged(double arg1)
{
    if (changeable) {
        if (ui->mygl->getClosestLatticeVertex() != NULL) {
            LatticeVertex * l = ui->mygl->getClosestLatticeVertex();
            glm::vec4 l_pos = l->getPosition();
            glm::vec4 new_l_pos4 = glm::vec4(l_pos[0], l_pos[1], arg1, l_pos[3]);
            glm::vec3 new_l_pos3 = glm::vec3 (l_pos[0], l_pos[1], arg1);
            glm::mat4 lTransform = glm::translate(glm::mat4(1.0f), new_l_pos3)*glm::scale(glm::mat4(1.0f), glm::vec3(0.1, 0.1, 0.1));
            l->setPosition(new_l_pos4);
            l->setTransformationMatrix(lTransform);

            std::cout << ui->mygl->getClosestLatticeVertex()->getLatticeVertices().size() << std::endl;
            for (Vertex* v : ui->mygl->getClosestLatticeVertex()->getLatticeVertices()) {
                v->setPoint_pos(new_l_pos4);
                v->setPos(new_l_pos4);
            }
        }
        for (wirebox* wb : ui->mygl->latticeCells) {
            wb->update();
        }
        ui->mygl->deformMesh();
        ui->mygl->update();
    }
}

void MainWindow::slot_populateLatticeSpinboxes() {
    if (ui->mygl->getClosestLatticeVertex()) {
        LatticeVertex * l = ui->mygl->getClosestLatticeVertex();
        glm::vec4 l_pos = l->getPosition();
        changeable = false;
        ui->latticeX_spinbox->setValue(l_pos[0]);
        ui->latticeY_spinbox->setValue(l_pos[1]);
        ui->latticeZ_spinbox->setValue(l_pos[2]);
        changeable = true;
    }
}

void MainWindow::on_deform_slider_sliderMoved(int position)
{
    ui->mygl->specialLatticeDeformation(position, deformType, deformAxis);
}

void MainWindow::on_radioButton_bend_clicked()
{
    deformType = 0;
}

void MainWindow::on_radioButton_taper_clicked()
{
    deformType = 1;
}

void MainWindow::on_radioButton_twist_clicked()
{
    deformType = 2;
}

void MainWindow::on_radioButton_squash_clicked()
{
    deformType = 3;
}
int MainWindow::getDeformAxis() const
{
    return deformAxis;
}

void MainWindow::setDeformAxis(int value)
{
    deformAxis = value;
}

int MainWindow::getDeformType() const
{
    return deformType;
}

void MainWindow::setDeformType(int value)
{
    deformType = value;
}


void MainWindow::on_radioButton_axis_x_clicked()
{
    deformAxis = 0;

}

void MainWindow::on_radioButton_axis_y_clicked()
{
    deformAxis = 1;
}

void MainWindow::on_radioButton_axis_z_clicked()
{
    deformAxis = 2;
}
