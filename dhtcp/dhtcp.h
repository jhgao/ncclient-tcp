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

#define WAIT_RCV_BLOCK_TIMEOUT 5000

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
    void onDataSktReadyRead();
private:
    bool isReadyToFetch();
    void writeOutCmd(eCMD, const QByteArray& = QByteArray());
    void processCMD(const Packet& p);
    QString psCmdDbg(QString cmd, QString arg = QString());
    void processData(const Packet& p);

    QString i_ipAddress;    //local ip
    QTcpSocket* i_tcpDataSkt;
    QTcpServer i_dataServer;
    int i_cmd_counter;
};
}

#endif // DHTCP_H
