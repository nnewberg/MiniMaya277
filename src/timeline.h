#ifndef TIMELINE_H
#define TIMELINE_H

#include <QListWidget>
#include "frame.h"
#include "joint.h"
class Timeline : public QListWidget
{
    Q_OBJECT
private:
    Joint* selectedJoint;
public:
    explicit Timeline(QWidget *parent = 0);
    void setSelectedJoint(Joint* joint);
    Joint* getSelectedJoint();
signals:

public slots:

};

#endif // TIMELINE_H
