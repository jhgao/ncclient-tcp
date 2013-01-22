#include "dhtcp.h"
namespace DHtcp{
DHtcp::DHtcp(QObject *parent) :
    DataHandler(parent),i_tcpDataSkt(0),i_dataServer(0),
    i_cmd_counter(0),i_decoder(0)
{
    i_decoder = new DHtcpDecoder(this);

    i_dataServer = new QTcpServer(this);
    if (!i_dataServer->listen(QHostAddress::Any,0)) {
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
             << ":" << i_dataServer->serverPort();

    connect(i_dataServer, SIGNAL(newConnection()),
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
    out.setVersion(QDataStream::Qt_4_8);
    out << i_ipAddress;
    out << (quint16) i_dataServer->serverPort();
    return arg;
}

void DHtcp::startFetch()
{
    if( this->isReadyToFetch() ){
        qDebug() << "DHtcp::startFetch()";
        writeOutCmd(CMD_START);
    }
}

void DHtcp::abortWorks()
{
    i_tcpDataSkt->abort();
}

void DHtcp::onIncomingDataConnection()
{
    if( i_dataServer->hasPendingConnections()){
        i_tcpDataSkt = i_dataServer->nextPendingConnection();
//        i_dataServer->close();
        qDebug() << "DHtcp::onIncomingDataConnection()";
        emit sig_dataConnected();
        //TODO check incoming identity
        connect(i_tcpDataSkt, SIGNAL(readyRead()),
                this, SLOT(onDataSktReadyRead()));
        connect(i_tcpDataSkt, SIGNAL(disconnected()),
                this, SLOT(onDataSktDisconnected()));
    }
}

void DHtcp::onDataSktReadyRead()
{
    QFile rcvFile(i_decoder->getRcvCacheFileName());

    if(!rcvFile.open(QIODevice::WriteOnly | QIODevice::Append)){
        qDebug() << "DHtcpDecoder: failed open cache file";
    }

    while (i_tcpDataSkt->bytesAvailable() > 0){
        QByteArray a = i_tcpDataSkt->read(
                    i_tcpDataSkt->bytesAvailable());
        rcvFile.write(a);
    }

    rcvFile.close();
}

void DHtcp::onDataSktDisconnected()
{
    qDebug() << "DHtcp::onDataSktDisconnected()";
    i_cmd_counter = 0;
}

bool DHtcp::isReadyToFetch()
{
    if( !i_tcpDataSkt ) return false;
    if( i_tcpDataSkt->isOpen() ){
        qDebug() << "\t data skt ready";
        return true;
    }
    return false;
}

void DHtcp::writeOutCmd(eCMD cmd, const QByteArray &arg)
{
    if(!i_tcpDataSkt) return;

    Packet p(cmd,arg);
    i_tcpDataSkt->write(p.genPacket());
}

void DHtcp::processCMD(const Packet &p)
{
    ++i_cmd_counter;

    switch(p.getCMD()){
    case CMD_START:
        psCmdDbg("CMD_START");
        break;
    case CMD_STOP:
        psCmdDbg("CMD_STOP");
        break;
    case FILE_SENT:
        psCmdDbg("FILE_SENT");
        i_decoder->flushCache();
        break;
    default:
        psCmdDbg(QString::number(p.getCMD()) + "?UNKNOWN" );
    }
}

QString DHtcp::psCmdDbg(QString cmd, QString arg)
{
    QString dbg;
    dbg = " [DHtcp] cmd " + QString::number(i_cmd_counter);
    dbg += " [" + cmd + "] ";
    dbg += arg;
    dbg += "\tfrom " + i_tcpDataSkt->peerAddress().toString()
            + " time " + QTime::currentTime().toString();

    qDebug() << dbg;
    return dbg;
}

void DHtcp::processData(const Packet &p)
{
    i_decoder->queueFileBlock(p.getData());
}

}
