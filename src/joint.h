#ifndef JOINT_H
#define JOINT_H

#include <vector>
#include "la.h"
#include <QTreeWidgetItem>

class Joint : public QTreeWidgetItem
{
public:
    Joint();

    static std::vector<Joint*> globalJoints;

    glm::mat4 getLocalTransformation();
    glm::mat4 getOverallTransformation();

    glm::mat4 getBindMatrix() const;
    void setBindMatrix(const glm::mat4 &value);

    glm::vec3 getPosition() const;
    void setPosition(const glm::vec3 &value);

    void addChild(QTreeWidgetItem *child);
    std::vector<QTreeWidgetItem *> getChildren() const;
    void setChildren(const std::vector<QTreeWidgetItem *> &value);

    QTreeWidgetItem *getParent() const;
    void setParent(Joint *value);

    QString getName() const;
    void setName(const QString &value);

    glm::fquat getRotation() const;
    void setRotation(const glm::fquat &value);

    glm::fquat createRotationQuat(float a, float x, float y, float z);

    float getDistToVertex() const;
    void setDistToVertex(float value);

    bool operator<(const Joint &vj2) const;


    glm::vec4 getColor() const;
    void setColor(const glm::vec4 &value);

    int getId() const;
    void setId(int value);

private:
    QString name;
    QTreeWidgetItem* parent;
    std::vector<QTreeWidgetItem *> children;
    glm::vec3 position; //relative to parent
    glm::fquat rotation;
    glm::mat4 bindMatrix;

    int id;

    float distToVertex;
    glm::vec4 color;

};

#endif // JOINT_H
