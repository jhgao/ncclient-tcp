#ifndef DHTCP_H
#define DHTCP_H

#include <QDataStream>
#include <QTcpSocket>
#include "datahandler.h"
#include "protocol/cmd_define.h"

class DHtcp : public DataHandler
{
    Q_OBJECT
public:
    explicit DHtcp(QObject *parent = 0);
    QByteArray getInitAckArg();
signals:
    
public slots:
private:
    QTcpSocket m_tcpDataSkt;
};

#endif // DHTCP_H
