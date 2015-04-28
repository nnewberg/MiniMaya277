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
    Frame* getLeftBoundedKeyFrame(Frame* frame, Joint* joint);
    Frame* getRightBoundedKeyFrame(Frame* frame, Joint *joint);
public:
    explicit Timeline(QWidget *parent = 0);
    void setSelectedJoint(Joint* joint);
    Joint* getSelectedJoint();
    glm::vec3 lerpFrame(Frame* frame);
    glm::vec3 getSlerpFrame(Frame* frame, Joint* joint);
    glm::quat getSquadFrame(Frame* frame, Joint* joint);
    void play();
signals:
    void sig_updateMyGL();
public slots:
    void slot_drawNextFrame();
private slots:


};

#endif // TIMELINE_H
