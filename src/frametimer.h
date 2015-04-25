#ifndef FRAMETIMER_H
#define FRAMETIMER_H
#include <QTimer>
class frameTimer : public QObject
{
    Q_OBJECT
public:
    frameTimer();
    QTimer *timer;
    int val;
public slots:
    void slot_updateTimerVal();
};

#endif // FRAMETIMER_H
