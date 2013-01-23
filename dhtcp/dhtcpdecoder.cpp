#include "dhtcpdecoder.h"
#include <QDebug>
namespace nProtocTCP{
DHtcpDecoder::DHtcpDecoder(QObject *parent) :
    QObject(parent),i_savedBytes(0),i_cacheSize(0),
    i_flushDelayTimer(0)
{
    i_flushDelayTimer = new QTimer(this);
    i_flushDelayTimer->setSingleShot(true);
    connect(i_flushDelayTimer,SIGNAL(timeout()),
            this,SLOT(flushCache()));

    i_rcvCacheFileInfo.setFile(RCV_CACHE_FILE_NAME);
    if(i_rcvCacheFileInfo.exists())
        QFile::remove(i_rcvCacheFileInfo.absoluteFilePath());

    if(!touch(i_rcvCacheFileInfo.absoluteFilePath())){
        qDebug() << "DHtcpDecoder: failed creat cache file";
    }
    i_rcvCacheFile.setFileName(i_rcvCacheFileInfo.absoluteFilePath());
}

bool DHtcpDecoder::queueFileBlock(const QByteArray &a)
{
    i_queue.append(a);
    i_cacheSize += a.size();

    if( i_cacheSize > CACHE_SIZE ) return flushCache();
    i_flushDelayTimer->start(FLUSH_DELAY);
    return true;
}

QString DHtcpDecoder::getRcvCacheFileName() const
{
    return i_rcvCacheFileInfo.absoluteFilePath();
}

bool DHtcpDecoder::touch(QString aFilePath)
{
    if( QFile::exists(aFilePath)) return true;

    QFile f(aFilePath);
    bool rst = f.open(QIODevice::ReadWrite);
    f.close();
            return rst;
}

bool DHtcpDecoder::flushCache()
{
    qDebug() << "DHtcpDecoder::flushCache()"
            << "queue size" << i_queue.size();

    if(!i_rcvCacheFile.open(QIODevice::WriteOnly | QIODevice::Append)){
        qDebug() << "DHtcpDecoder: failed open cache file";
        return false;
    }
    qint64 lastOffset = 0;

    while(!i_queue.isEmpty()){
        QByteArray a = i_queue.dequeue();

        RawBlock b;
        b.fromArray(a);

        if( 0 == b.offsetTo) break;
        if ( b.offsetFrom != b.offsetTo ) lastOffset = b.offsetTo;

        i_rcvCacheFile.seek(b.offsetFrom);
        qint64 wroteBytes = i_rcvCacheFile.write(b.data);

        if(wroteBytes == b.data.size()){
            i_savedBytes += wroteBytes;
        }else{
            qDebug() << "DHtcpDecoder: saved file block corrupted";
            emit sig_blockCorruptOffset(b.offsetFrom);
        }

        if( i_savedBytes == b.fileSize ){
            emit sig_savedAllBytes();
            qDebug() << "saved a file of size" << i_savedBytes;
        }
    }

    qDebug() << "\t last offset" << lastOffset;
    i_rcvCacheFile.close();
    i_cacheSize = 0;
    return true;
}

}

