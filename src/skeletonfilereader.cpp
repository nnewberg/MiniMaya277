#include "skeletonfilereader.h"

SkeletonFileReader::SkeletonFileReader()
{
    rootJoint = NULL;
    idCount = 0;
}

bool SkeletonFileReader::readJson(Joint* rootJoint) {
    QString filename = QFileDialog::getOpenFileName(0, QString(""), QString("../../"), QString("*.json"));
    if (filename.size() <= 0) {
        std::cout << "no file specified" << std::endl;
        return false;
    }

    QFile jsonFile(filename);
    if (!jsonFile.exists()) {
        std::cout << "file doesnt exist" << std::endl;
        return false;
    }
    jsonFile.open(QIODevice::ReadOnly);

    QString jsonContent = jsonFile.readAll();
    jsonFile.close();

    QJsonDocument js = QJsonDocument::fromJson(jsonContent.toUtf8());
    if (js.isObject()) {
        QJsonObject o = js.object();
        QJsonValue rv = o.value("root");
        QJsonObject root = rv.toObject();
        rootJoint->setId(idCount);
        idCount++;
        parseObject(root, rootJoint);
        return true;
    }
    return false;
}

void SkeletonFileReader::parseObject(QJsonObject root, Joint* j) {
    QString name = root.value("name").toString();
    j->setName(name);
    QJsonArray pos = root.value("pos").toArray();
    glm::vec3 position = glm::vec3(pos.at(0).toDouble(), pos.at(1).toDouble(), pos.at(2).toDouble());

    j->setPosition(position);
    QJsonArray rot = root.value("pos").toArray();
    glm::fquat rq = j->createRotationQuat(rot.at(0).toDouble(), rot.at(1).toDouble(), rot.at(2).toDouble(), rot.at(3).toDouble());
    j->setRotation(rq);

    QJsonArray children = root.value("children").toArray();
    for (unsigned long i = 0; i < children.size(); i++) {
        QJsonObject c = children.at(i).toObject();
        Joint* cj = new Joint();
        cj->setParent(j);
        cj->setId(idCount);
        idCount++;
        j->addChild(cj);
        parseObject(c, cj);
    }
}

Joint* SkeletonFileReader::getRoot() {
    return rootJoint;
}


