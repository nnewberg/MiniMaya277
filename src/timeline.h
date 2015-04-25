#ifndef TIMELINE_H
#define TIMELINE_H

#include <QListWidget>
#include <QTimer>
#include "frame.h"
#include "joint.h"
class Timeline : public QListWidget
{
    Q_OBJECT
private:
    Joint* selectedJoint;
    QTimer* frameTimer;
    int frameTimerVal;
public:
    explicit Timeline(QWidget *parent = 0);
    void setSelectedJoint(Joint* joint);
    Joint* getSelectedJoint();
    glm::vec3 lerpFrame(Frame* frame);
    void play();
signals:

public slots:
    void slot_drawNextFrame();
private slots:


};

#endif // TIMELINE_H
