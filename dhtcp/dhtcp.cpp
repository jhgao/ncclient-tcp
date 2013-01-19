#include "dhtcp.h"

DHtcp::DHtcp(QObject *parent) :
    DataHandler(parent)
{
}

eProtocTypes DHtcp::type() const
{
    return PROTOC_TCP;
}

QByteArray DHtcp::declareArg()
{
    return QByteArray();
}

void DHtcp::startFetch()
{
    qDebug() << "DHtcp::startFetch()";
}
