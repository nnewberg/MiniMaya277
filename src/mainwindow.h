#pragma once

#include <QMainWindow>
#include <QListWidgetItem>
#include <mesh.h>
#include "frame.h"

namespace Ui
{
class MainWindow;
}


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);



    ~MainWindow();

    Joint *getCurrentJoint() const;
    void setCurrentJoint(Joint *value);

    int getDeformType() const;
    void setDeformType(int value);

    int getDeformAxis() const;
    void setDeformAxis(int value);

public slots:
    void slot_populateVert(QListWidgetItem* i);
    void slot_addTreeRoot(QTreeWidgetItem * jRoot);

    void slot_populateLatticeSpinboxes();
    //<kerem>
    void slot_set_meshList();
    //</kerem>

private slots:

    void on_actionQuit_triggered();

    void on_edge_listWidget_itemClicked(QListWidgetItem *item);

    void on_vertex_listWidget_itemClicked(QListWidgetItem *item);

    void on_face_listWidget_itemClicked(QListWidgetItem *item);

    void on_mygl_sig_populateVert(QListWidgetItem * item);

    void on_mygl_sig_populateFace(QListWidgetItem * item);

    void on_mygl_sig_populateEdge(QListWidgetItem * item);

    void on_doubleSpinBox_valueChanged(double arg1);

    void on_doubleSpinBox_2_valueChanged(double arg1);

    void on_doubleSpinBox_3_valueChanged(double arg1);

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_doubleSpinBox_4_valueChanged(double arg1);

    void on_doubleSpinBox_5_valueChanged(double arg1);

    void on_doubleSpinBox_6_valueChanged(double arg1);

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_checkBox_stateChanged(int arg1);

    void on_pushButton_6_clicked();



    void on_treeWidget_itemClicked(QTreeWidgetItem *item, int column);

    void on_treeWidget_itemSelectionChanged();

    void on_doubleSpinBox_11_valueChanged(double arg1);

    void on_doubleSpinBox_10_valueChanged(double arg1);

    void on_doubleSpinBox_12_valueChanged(double arg1);

    void on_doubleSpinBox_9_valueChanged(double arg1);

    void on_doubleSpinBox_7_valueChanged(double arg1);

    void on_doubleSpinBox_8_valueChanged(double arg1);

    void on_pushButton_7_clicked();

    void on_timeline_listWidget_itemPressed(QListWidgetItem *item);

    void on_latticeX_spinbox_valueChanged(double arg1);

    void on_latticeY_spinbox_valueChanged(double arg1);

    void on_latticeZ_spinbox_valueChanged(double arg1);



    void on_deform_slider_sliderMoved(int position);

    void on_radioButton_bend_clicked();

    void on_radioButton_taper_clicked();

    void on_radioButton_twist_clicked();

    void on_radioButton_squash_clicked();

    void on_radioButton_axis_x_clicked();

    void on_radioButton_axis_y_clicked();

    void on_radioButton_axis_z_clicked();

private:
    Ui::MainWindow *ui;

    Vertex* currentVertex;
    Face* currentFace;

    Joint* currentJoint;

    Frame* currentFrame;

    bool changeable;

    float rx, ry, rz;

    int deformType;
    int deformAxis;
};
