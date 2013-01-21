#include "connectionthread.h"

ConnectionThread::ConnectionThread(QObject *parent) :
    QThread(parent)
{
}

void ConnectionThread::run()
{
    qDebug() << "ConnectionThread::run()" << this->thread();
    exec();
}
