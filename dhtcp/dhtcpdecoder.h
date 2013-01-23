#ifndef DHTCPDECODER_H
#define DHTCPDECODER_H

#include <QObject>
#include <QFileInfo>
#include <QFile>
#include <QQueue>
#include <QTimer>
#include "protocol/rawblock.h"

#define CACHE_SIZE (500*1000) //500K
#define RCV_CACHE_FILE_NAME "rcv.cache"
#define FILE_WAIT_BYTES_WRITTEN 5000    //5S
#define FLUSH_DELAY 1000 // 1S

namespace nProtocTCP{
class DHtcpDecoder : public QObject
{
    Q_OBJECT
public:
    explicit DHtcpDecoder(QObject *parent = 0);
    bool queueFileBlock(const QByteArray &);
    QString getRcvCacheFileName()const;
signals:
    void sig_savedAllBytes();
    void sig_blockCorruptOffset(quint64);

public slots:
    bool flushCache();

private:
    bool touch(QString aFilePath);      //creat a certain file

    QFileInfo i_rcvCacheFileInfo;
    QFile i_rcvCacheFile;
    qint64 i_savedBytes;
    QQueue<QByteArray> i_queue; //cache
    qint64 i_cacheSize;
    QTimer* i_flushDelayTimer;
};
}

#endif // DHTCPDECODER_H
