#include "meshlistwidget.h"
#include <iostream>

meshListWidget::meshListWidget(QWidget* p) : QListWidget(p)
{
}

void meshListWidget::slot_populateVert(std::vector<QListWidgetItem *> *l) {
    std::cout << "inserted" << std::endl;
    for (unsigned long i = 0; i < l->size(); i++) {
        this->insertItem(i, l->at(i));
        std::cout << "inserted" << i << std::endl;
    }
}

void meshListWidget::slot_populateFace(std::vector<QListWidgetItem *> *l) {
    std::cout << "inserted" << std::endl;
    for (unsigned long i = 0; i < l->size(); i++) {
        this->insertItem(i, l->at(i));
    }
}

void meshListWidget::slot_populateEdge(std::vector<QListWidgetItem *> *l) {
    std::cout << "inserted" << std::endl;
    for (unsigned long i = 0; i < l->size(); i++) {
        this->insertItem(i, l->at(i));
    }
}
