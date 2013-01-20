#include "packet.h"

Packet::Packet(eControl_CMD cmd,const QByteArray arg):
    i_type(PTYPE_CMD),i_cmd(cmd),i_cmd_arg(arg)
{
}

Packet::Packet(const QByteArray &data) :
    i_type(PTYPE_DATA),i_data(data)
{
}

Packet::Packet()
{
}

const QByteArray &Packet::genPacket()
{
    i_payload.clear();
    QDataStream payloads(&i_payload, QIODevice::WriteOnly);
    payloads.setVersion(QDataStream::Qt_4_0);
    switch(i_type){
    case PTYPE_CMD:
        payloads << (quint16)PTYPE_CMD;  //data: packate type
        payloads << (quint16)i_cmd;    //data: packate data
        payloads << i_cmd_arg;
        break;
    case PTYPE_DATA:
        payloads << (quint16)PTYPE_DATA;
        payloads << i_data; //.data();
        break;
    default:
        ;
    }

    i_packet.clear();
    QDataStream out(&i_packet, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_0);
    out << (quint16)0;  //preserve for packet size
    out << i_payload;
    out.device()->seek(0);
    out << (quint16)(i_packet.size() - sizeof(quint16));

    return i_packet;
}

bool Packet::fromPacket(QByteArray a)
{
    i_packet = a;
    quint16 blockSize;
    QDataStream in(&i_packet,QIODevice::ReadOnly);
    in.setVersion(QDataStream::Qt_4_0);
    in >> blockSize;

    if( (quint16)a.size()-sizeof(quint16) < blockSize)
        return false;

    i_payload.clear();
    in >> i_payload;
    return this->fromPayload(i_payload);
}

bool Packet::fromPayload(QByteArray a)
{
    i_payload = a;
    quint16 type, cmd;
    QDataStream in(&i_payload,QIODevice::ReadOnly);
    in.setVersion(QDataStream::Qt_4_0);
    in >> type;
    i_type = (ePacketType)type;

    switch(i_type){
    case PTYPE_CMD:
        in >> cmd;
        in >> i_cmd_arg;
        i_cmd = (eControl_CMD)cmd;
        break;
    case PTYPE_DATA:
        in >> i_data;
        break;
    default:
        return false;
    }
    return true;
}

QString Packet::toString()
{
    QString s;
    switch(i_type){
    case PTYPE_CMD:
        switch (i_cmd){
        case CON_CONNECTED: s = "CON_CONNECTED"; break;
        case CON_CONNECTING: s = "CON_CONNECTIONG"; break;
        case CON_D5F: s = "CON_D5F"; break;
        case CON_D5FACK: s = "CON_D5FACK"; break;
        case CON_NEXT: s = "CON_NEXT"; break;
        case CON_START: s = "START"; break;
        }
        return s;
        break;
    case PTYPE_DATA:
        return QString(i_data.toHex());
        break;
    default:
        return QString();
    }
}

ePacketType Packet::getType() const
{
    return i_type;
}

eControl_CMD Packet::getCMD()const
{
    return i_cmd;
}

QByteArray Packet::getCMDarg()const
{
    return i_cmd_arg;
}

QByteArray Packet::getData() const
{
    return i_data;
}
