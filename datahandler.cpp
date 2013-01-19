#include "datahandler.h"

DataHandler::DataHandler(QObject *parent) :
    QObject(parent)
{
}


void DataHandler::sigWriteOutCmd(const eControl_CMD cmd, const QByteArray arg)
{
    emit sig_writeOutCmd(cmd,arg);
}
