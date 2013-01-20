#include "dhtcp.h"
namespace DHtcp{
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
    return QByteArray("DHtcp"); //TODO data port
}

void DHtcp::startFetch()
{
    qDebug() << "DHtcp::startFetch()";
}

}
