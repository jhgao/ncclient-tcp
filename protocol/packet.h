/*          quit16              payloadLength
 *          <payloadLength>      <payload>
 *  payload = type + cmd
 *  or      = type + data
 */

#ifndef PACKET_H
#define PACKET_H

#include <QtNetwork>
#include <QDataStream>
#include "cmd_define.h"

class Packet
{
public:
    explicit Packet(eControl_CMD cmd,
                    const QByteArray arg = QByteArray()); //cmd packet

    explicit Packet(const QByteArray &data); //data packet

    explicit Packet();

    const QByteArray& genPacket();

    bool fromPacket(QByteArray a);
    bool fromPayload(QByteArray a);
    QString toString();

    ePacketType getType() const;
    eControl_CMD getCMD() const;
    QByteArray getCMDarg() const;
    QByteArray getData()const;

private:
    ePacketType i_type;

    eControl_CMD i_cmd;
    QByteArray i_cmd_arg;

    QByteArray i_data;

    QByteArray i_payload;
    QByteArray i_packet;
};

#endif // PACKET_H
