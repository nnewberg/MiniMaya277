#include "frame.h"

uint Frame::idCount = 0;

Frame::Frame()
{
    this->keyframed = false;
    Frame::idCount++;
    this->setText(QString::number(idCount));
}

void Frame::setJointPosRot(Joint* joint, glm::vec3 pos, glm::fquat rot){
    this->jointToPosRotMap.insert(joint, std::pair<glm::vec3, glm::fquat>(pos, rot));
}

void Frame::setJointPos(Joint* joint, glm::vec3 pos){
    this->jointToPosRotMap.insert(joint, std::pair<glm::vec3, glm::fquat>(pos, getJointRot(joint)));
}

glm::vec3 Frame::getJointPos(Joint* joint){
    return this->jointToPosRotMap[joint].first;
}

void Frame::setJointRot(Joint *joint, glm::quat rot){
    this->jointToPosRotMap.insert(joint, std::pair<glm::vec3, glm::fquat>(getJointPos(joint), rot));
}

glm::fquat Frame::getJointRot(Joint* joint){
    return this->jointToPosRotMap[joint].second;
}

bool Frame::isKeyFrame(){
    return this->keyframed;
}

void Frame::setKeyFrame(){
    this->keyframed = true;
}

bool Frame::animatesJoint(Joint* joint){
    return this->jointToPosRotMap.contains(joint);
}
