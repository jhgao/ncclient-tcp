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
        writeOutCmd(CMD_START);
    }
}

void DHtcp::onIncomingDataConnection()
{
    if( i_dataServer.hasPendingConnections()){
        i_tcpDataSkt = i_dataServer.nextPendingConnection();
//        i_dataServer.close();
        qDebug() << "DHtcp::onIncomingDataConnection()";
        emit sig_dataConnected();
        //TODO check incoming identity
        connect(i_tcpDataSkt, SIGNAL(readyRead()),
                this, SLOT(onDataSktReadyRead()));
    }
}

void DHtcp::onDataSktReadyRead()
{
    quint16 packetSize = 0;

    //get packet size
    QDataStream in(i_tcpDataSkt);
    in.setVersion(QDataStream::Qt_4_0);
    if (packetSize == 0) {
        if (i_tcpDataSkt->bytesAvailable() < (int)sizeof(quint16)){
            qDebug() << "\t E: packet size wrong"
                     << i_tcpDataSkt->bytesAvailable()
                     << "/"
                     << (int)sizeof(quint16);;
            return;
        }
        in >> packetSize;
    }

    //ensure data size available
    if (i_tcpDataSkt->bytesAvailable() < packetSize){
        qDebug() << "\t E: not enough data bytes"
                 << i_tcpDataSkt->bytesAvailable()
                 << "/need " << packetSize;
        return;
    }

    //read in data
    QByteArray payloadArrey;
    in >> payloadArrey;

    //analyze payload
    Packet p;
    if( p.fromPayload(payloadArrey)){
        switch(p.getType()){
        case PTYPE_CMD:
            processCMD(p);
            break;
        case PTYPE_DATA:
            qDebug() << payloadArrey.size();
            processData(p);
            break;
        default:
            qDebug() << "\t unknown packet type";
        }
    }
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
    default:
        psCmdDbg("? UNKNOWN ?");
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
    qDebug() << "DHtcp::processData()"
             << p.getData().toHex();
}

}