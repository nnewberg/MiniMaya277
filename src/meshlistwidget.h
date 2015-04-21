#ifndef MESHLISTWIDGET_H
#define MESHLISTWIDGET_H

#include <QListWidget>
#include <QListWidgetItem>
#include <mygl.h>

class meshListWidget : public QListWidget
{
public:
    meshListWidget(QWidget* p);

public slots:
    void slot_populateVert(std::vector<QListWidgetItem*>* l);
    void slot_populateFace(std::vector<QListWidgetItem*>* l);
    void slot_populateEdge(std::vector<QListWidgetItem*>* l);
};



#endif // MESHLISTWIDGET_H
