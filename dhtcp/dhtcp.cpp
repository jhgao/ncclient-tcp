#include "dhtcp.h"

DHtcp::DHtcp(QObject *parent) :
    DataHandler(parent)
{
}

QByteArray DHtcp::getInitAckArg()
{
    QByteArray arg;
    QDataStream out(&arg, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_0);
    out << (quint16)15;
    return arg;
}
