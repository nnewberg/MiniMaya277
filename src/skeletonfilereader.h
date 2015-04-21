#ifndef SKELETONFILEREADER_H
#define SKELETONFILEREADER_H

#include <QFile>
#include <QDialog>
#include <QFileDialog>
#include <QTextStream>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <stdio.h>
#include <iostream>
#include <QTreeWidgetItem>
#include "joint.h"

class SkeletonFileReader
{
public:
    SkeletonFileReader();
    bool readJson(Joint* rootJoint);
    void parseObject(QJsonObject o, Joint* j);
    Joint* getRoot();

private:
    Joint* rootJoint;
    int idCount;
};

#endif // SKELETONFILEREADER_H
