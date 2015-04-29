#include "timeline.h"
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

Frame* Timeline::getLeftBoundedKeyFrame(Frame* frame, Joint *joint){
    Frame* currFrame = frame;
    while(!currFrame->isKeyFrameForJoint(joint)){
        //qWarning()<<"Current Index: "<<currFrame->index;
        if (currFrame->index == 0){
            //qWarning()<<"Cant find left bounded keyframe";
            currFrame = nullptr;
            break;
        }
        currFrame = (Frame*)(this->item(currFrame->index - 1));
    }
    return currFrame;
}

Frame* Timeline::getRightBoundedKeyFrame(Frame* frame, Joint* joint){
    Frame* currFrame = frame;
    while(!currFrame->isKeyFrameForJoint(joint)){
        //qWarning()<<"Current Index: "<<currFrame->index;
        if (currFrame->index == (Timeline::count() -1)){
            //qWarning()<<"Cant find right bounded keyframe";
            currFrame = nullptr;
            break;
        }
        currFrame = (Frame*)(this->item(currFrame->index + 1));
    }
    return currFrame;
}

glm::vec3 lerp(float t, glm::vec3 a, glm::vec3 b){
   return ((1-t)*a + t*b);
}

glm::vec3 Timeline::getSlerpFrame(Frame* frame, Joint* joint){
    Frame* p0_frame;
    Frame* p1_frame;
    Frame* p2_frame;
    Frame* p3_frame;

    p1_frame = getLeftBoundedKeyFrame(frame, joint);
    if (p1_frame == nullptr){
        return joint->getPosition();
    }
    p0_frame = getLeftBoundedKeyFrame(p1_frame, joint);
    if (p0_frame == nullptr){
        p0_frame = p1_frame;
    }

    p2_frame = getRightBoundedKeyFrame(frame, joint);
    if(p2_frame == nullptr){
        return joint->getPosition();
    }
    p3_frame = getRightBoundedKeyFrame(p2_frame, joint);
    if(p2_frame == nullptr){
        p3_frame = p2_frame;
    }

    float t = ((float)(frame->index - p1_frame->index))/(p2_frame->index
                                                        - p1_frame->index);

    glm::vec3 q0 = lerp(t, p0_frame->getJointPos(joint), p1_frame->getJointPos(joint));
    glm::vec3 q1 = lerp(t, p1_frame->getJointPos(joint), p2_frame->getJointPos(joint));
    glm::vec3 q2 = lerp(t, p2_frame->getJointPos(joint), p3_frame->getJointPos(joint));

    glm::vec3 r0 = lerp(t, q0, q1);
    glm::vec3 r1 = lerp(t, q1, q2);

    glm::vec3 smoothPos = lerp(t, r0, r1);

    return smoothPos;
}

glm::quat Timeline::getSquadFrame(Frame* frame, Joint* joint){

    Frame* p0_frame;
    Frame* p1_frame;
    Frame* p2_frame;
    Frame* p3_frame;

    p1_frame = getLeftBoundedKeyFrame(frame, joint);
    if (p1_frame == nullptr){
        return joint->getRotation();
    }
    p0_frame = getLeftBoundedKeyFrame(p1_frame, joint);
    if (p0_frame == nullptr){
        p0_frame = p1_frame;
    }

    p2_frame = getRightBoundedKeyFrame(frame, joint);
    if(p2_frame == nullptr){
        return joint->getRotation();
    }
    p3_frame = getRightBoundedKeyFrame(p2_frame, joint);
    if(p2_frame == nullptr){
        p3_frame = p2_frame;
    }

    float t = ((float)(frame->index - p1_frame->index))/(p2_frame->index
                                                        - p1_frame->index);

    return glm::squad(p0_frame->getJointRot(joint), p3_frame->getJointRot(joint),
               p1_frame->getJointRot(joint), p2_frame->getJointRot(joint), t);
}


glm::vec3 Timeline::lerpFrame(Frame* frame){

    //find left bounded frame
    Frame* leftBoundedFrame = getLeftBoundedKeyFrame(frame, selectedJoint);

    //find right bounded frame
    Frame* rightBoundedFrame = getRightBoundedKeyFrame(frame, selectedJoint);

    if (leftBoundedFrame == nullptr || rightBoundedFrame == nullptr){
        return selectedJoint->getPosition();
    }
    //qWarning()<<"Left Bounded Idx: "<<leftBoundedFrame->index;
    //qWarning()<<"Right Bounded Idx: "<<rightBoundedFrame->index;
    //calculate Time t
    float t = ((float)(frame->index - leftBoundedFrame->index))/(rightBoundedFrame->index
                                                        - leftBoundedFrame->index);
    //qWarning()<<"Time = "<<t

    glm::vec3 smoothPos = lerp(t, leftBoundedFrame->getJointPos(selectedJoint),
                               rightBoundedFrame->getJointPos(selectedJoint));
//    glm::vec3 smoothPos =  (1-t)*leftBoundedFrame->getJointPos(selectedJoint) +
//            t*rightBoundedFrame->getJointPos(selectedJoint);

    return smoothPos;
}

void Timeline::play(){
    this->frameTimer->start(100);
}

void Timeline::slot_drawNextFrame(){
    qDebug()<<"New timer!";
    qDebug()<<"Val: "<<frameTimerVal;
    Frame* currFrame = (Frame*)this->item(frameTimerVal);
    //this->itemPressed(currFrame);
    std::vector<Joint*> joints = Joint::globalJoints;
    for(uint i = 0; i < joints.size(); i++){
        Joint* currJoint = joints[i];
        glm::vec3 newPos;
        glm::quat newRot;
        if (currFrame->isKeyFrameForJoint(currJoint)){
            newPos = currFrame->getJointPos(currJoint);
            newRot = currFrame->getJointRot(currJoint);
        }else{
            newPos = getSlerpFrame(currFrame, currJoint);
            newRot = getSquadFrame(currFrame, currJoint);
        }

        currJoint->setPosition(newPos);
        currJoint->setRotation(newRot);

        sig_updateMyGL();
    }

    if(frameTimerVal != count() - 1){
        frameTimerVal++;
    }else{
        this->frameTimer->stop();
        this->frameTimerVal = 0;
    }
}



