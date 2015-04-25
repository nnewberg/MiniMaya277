#include "frametimer.h"
#include <QDebug>
frameTimer::frameTimer()
{
   // create a timer
   timer = new QTimer(this);

   //set value
   val = 0;

   // setup signal and slot
   connect(timer, SIGNAL(timeout()),
         this, SLOT(slot_updateTimerVal()));

   // msec
   timer->start(1000);
}

void frameTimer::slot_updateTimerVal(){
    qDebug()<<"Update!";
    val++;
}
