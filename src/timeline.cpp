#include "timeline.h"
#include "frametimer.h"
#include "mainwindow.h"

Timeline::Timeline(QWidget *parent) :
    QListWidget(parent)
{
    this->selectedJoint = nullptr;

    //setup timer
    frameTimer = new QTimer(this);
    connect(frameTimer, SIGNAL(timeout()),
          this, SLOT(slot_drawNextFrame()));
    frameTimerVal = 0;

    for (uint i = 0; i < 10; i++){
        Frame* frame = new Frame();
        //frame->setText(QString("Frame "));
        this->addItem(frame);
    }
}

void Timeline::setSelectedJoint(Joint* joint){
    this->selectedJoint = joint;
}

Joint* Timeline::getSelectedJoint(){
    return this->selectedJoint;
}

glm::vec3 Timeline::lerpFrame(Frame* frame){
    //find left bounded frame
    //qWarning()<<"Timeline[0] index = "<<(((Frame*)(this->itemAt(0,0)))->index);
    //qWarning()<<"Timeline[1] index = "<<(((Frame*)(this->item(1)))->index);
    Frame* currFrame = frame;
    while(!currFrame->isKeyFrameForJoint(selectedJoint)){
        //qWarning()<<"Current Index: "<<currFrame->index;
        if (currFrame->index == 0){
            //qWarning()<<"Cant find left bounded keyframe";
            currFrame = nullptr;
            break;
        }
        currFrame = (Frame*)(this->item(currFrame->index - 1));
    }
    Frame* leftBoundedFrame = currFrame;
   //find right bounded frame
   //qWarning()<<"";
    currFrame = frame;
    while(!currFrame->isKeyFrameForJoint(selectedJoint)){
        //qWarning()<<"Current Index: "<<currFrame->index;
        if (currFrame->index == (Timeline::count() -1)){
            //qWarning()<<"Cant find right bounded keyframe";
            currFrame = nullptr;
            break;
        }
        currFrame = (Frame*)(this->item(currFrame->index + 1));
    }
    Frame* rightBoundedFrame = currFrame;
    if (leftBoundedFrame == nullptr || rightBoundedFrame == nullptr){
        return selectedJoint->getPosition();
    }
    //qWarning()<<"Left Bounded Idx: "<<leftBoundedFrame->index;
    //qWarning()<<"Right Bounded Idx: "<<rightBoundedFrame->index;
    //calculate Time t
    float t = ((float)(frame->index - leftBoundedFrame->index))/(rightBoundedFrame->index
                                                        - leftBoundedFrame->index);
    //qWarning()<<"Time = "<<t;

    glm::vec3 smoothPos =  (1-t)*leftBoundedFrame->getJointPos(selectedJoint) +
            t*rightBoundedFrame->getJointPos(selectedJoint);

    return smoothPos;
}

void Timeline::play(){
//    frameTimer* timer = new frameTimer();
//    Frame* currFrame;
//    int timerVal = timer->val;
//    qDebug()<<"Timer Val: "<<timer->val;
    this->frameTimer->start(1000);
}

void Timeline::slot_drawNextFrame(){
    qDebug()<<"New timer!";
    qDebug()<<"Val: "<<frameTimerVal;
    QListWidgetItem* currFrame = this->item(frameTimerVal);
    //glm::vec3 new_pos = lerpFrame(currFrame);
    //selectedJoint->setPosition(new_pos);
    this->itemPressed(currFrame);
    if(frameTimerVal != count() - 1){
        frameTimerVal++;
    }else{
        this->frameTimer->stop();
        this->frameTimerVal = 0;
    }
}



