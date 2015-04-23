#include "timeline.h"

Timeline::Timeline(QWidget *parent) :
    QListWidget(parent)
{
    this->selectedJoint = nullptr;

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


