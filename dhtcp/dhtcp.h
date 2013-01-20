#ifndef DHTCP_H
#define DHTCP_H

#include <QDataStream>
#include <QTcpSocket>
#include <QTcpServer>
#include <QNetworkInterface>
#include "datahandler.h"
#include "protocol/cmd_define.h"
#include "protocol/packet.h"

#include "dhtcpprotocol.h"

namespace DHtcp{
class DHtcp : public DataHandler
{
    Q_OBJECT
public:
    explicit DHtcp(QObject *parent = 0);
    eProtocTypes type() const;
    QByteArray declareArg();
signals:
    void sig_dataConnected();
    
public slots:
    void startFetch();

private slots:
    void onIncomingDataConnection();
private:
    bool isReadyToFetch();
    void writeOutCmd(eCMD, const QByteArray& = QByteArray());

    QString i_ipAddress;    //local ip
    QTcpSocket* i_tcpDataSkt;
    QTcpServer i_dataServer;
};
}

#endif // DHTCP_H
