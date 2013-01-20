#include "packet.h"

Packet::Packet(quint16 cmd, const QByteArray arg):
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
    QDataStream in(&i_payload,QIODevice::ReadOnly);
    in.setVersion(QDataStream::Qt_4_0);
    in >> i_type;

    switch(i_type){
    case PTYPE_CMD:
        in >> i_cmd;
        in >> i_cmd_arg;
        break;
    case PTYPE_DATA:
        in >> i_data;
        break;
    default:
        return false;
    }
    return true;
}

quint16 Packet::getType() const
{
    return i_type;
}

quint16 Packet::getCMD()const
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
