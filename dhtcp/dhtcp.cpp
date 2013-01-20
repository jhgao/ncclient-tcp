#include "dhtcp.h"
namespace DHtcp{
DHtcp::DHtcp(QObject *parent) :
    DataHandler(parent),i_tcpDataSkt(0)
{
    if (!i_dataServer.listen(QHostAddress::Any,0)) {
        qDebug() << "DHtcp listen data port failed";
        exit(-1);
    }

    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
    // use the first non-localhost IPv4 address
    for (int i = 0; i < ipAddressesList.size(); ++i) {
        if (ipAddressesList.at(i) != QHostAddress::LocalHost &&
            ipAddressesList.at(i).toIPv4Address()) {
            i_ipAddress = ipAddressesList.at(i).toString();
            break;
        }
    }
    // if we did not find one, use IPv4 localhost
    if (i_ipAddress.isEmpty())
        i_ipAddress = QHostAddress(QHostAddress::LocalHost).toString();

    qDebug() << "\n DHtcp is listening data at" << i_ipAddress
             << ":" << i_dataServer.serverPort();

    connect(&i_dataServer, SIGNAL(newConnection()),
            this, SLOT(onIncomingDataConnection()));

    //this logic
    connect(this, SIGNAL(sig_dataConnected()), this, SLOT(startFetch()));
}

eProtocTypes DHtcp::type() const
{
    return PROTOC_TCP;
}

QByteArray DHtcp::declareArg()  //local data port listening
{
    QByteArray arg;
    QDataStream out(&arg, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_0);
    out << i_ipAddress;
    out << (quint16) i_dataServer.serverPort();
    return arg;
}

void DHtcp::startFetch()
{
    if( this->isReadyToFetch() ){
        qDebug() << "DHtcp::startFetch()";
    }
}

void DHtcp::onIncomingDataConnection()
{
    if( i_dataServer.hasPendingConnections()){
        i_tcpDataSkt = i_dataServer.nextPendingConnection();
        i_dataServer.close();
        qDebug() << "DHtcp::onIncomingDataConnection() stop listening";
        emit sig_dataConnected();
    }
}

bool DHtcp::isReadyToFetch()
{
    if( !i_tcpDataSkt ) return false;
    if( i_tcpDataSkt->isOpen() ){
        qDebug() << "\t data skt ready";
        return true;
    }
}

void DHtcp::writeOutCmd(eCMD, const QByteArray &)
{
}

}
