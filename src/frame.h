#ifndef FRAME_H
#define FRAME_H
#include <QListWidgetItem>
#include "la.h"
#include "joint.h"



class Frame: public QListWidgetItem
{
private:
    QMap<Joint*, std::pair<glm::vec3, glm::fquat>> jointToPosRotMap; //maps a joint to its
                                                        //position and rotation on this frame
    bool keyframed;

public:
    Frame();
    static uint idCount;
    void setJointPosRot(Joint* joint, glm::vec3 pos, glm::fquat rot);
    void setJointPos(Joint* joint, glm::vec3 pos);
    glm::vec3 getJointPos(Joint* joint); //returns the joints position at this frame
    void setJointRot(Joint* joint, glm::quat rot);
    glm::fquat getJointRot(Joint* joint); //returns joints rotation at this frame
    bool isKeyFrame();
    void setKeyFrame(); //sets this frame as a key frame
    bool animatesJoint(Joint* joint); //returns true if frame maps contains joint
};

#endif // FRAME_H
