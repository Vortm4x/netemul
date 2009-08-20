#ifndef TCPPACKET_H
#define TCPPACKET_H

#include <QDataStream>
#include <QSharedData>

class tcpPacketData : public QSharedData
{
public:
    tcpPacketData() { }
    tcpPacketData(const tcpPacketData &other);
    ~tcpPacketData() { }
    QByteArray toData() const;
    friend class tcpPacket;
private:
    quint16 sender; //!< Порт отправителя
    quint16 receiver; //!< Порт получателя
    quint32 sequence;
    quint32 ack;
    quint8 flag;
    quint16 window;
    QByteArray data; //!< Поле данных
};

/*!
  Реализует tcp-сегмент
*/
class tcpPacket
{
public:
    enum { User = 777 , Window = 50 };
    enum { NO_FLAGS = 0, ACK = 1, FIN = 2 };
    tcpPacket() { d = new tcpPacketData; }
    tcpPacket(const QByteArray &b);
    tcpPacket(const tcpPacket &other) : d(other.d) { }
    ~tcpPacket() { }
    QByteArray toData() const { return d->toData(); }
    int size() const { return d->data.size(); }
    void setSender(quint16 s) { d->sender = s; }
    void setReceiver(quint16 r) { d->receiver = r; }
    void setSequence(quint32 s) { d->sequence = s; }
    void setAck(quint32 a) { d->ack = a; }
    void setFlag(quint8 f) { d->flag = f; }
    void setWindow(quint16 w) { d->window = w; }
    quint16 sender() { return d->sender; }
    quint16 receiver() { return d->receiver; }
    quint32 sequence() { return d->sequence; }
    quint32 ack() { return d->ack; }
    quint8 flag() { return d->flag; }
    quint16 window() { return d->window; }
    QByteArray unpack() const { return d->data; }
    void pack(QByteArray &b) { d->data = b; }
private:
    QSharedDataPointer<tcpPacketData> d;
protected:    
    friend QDataStream& operator<<( QDataStream &stream, const tcpPacket &p );
};

#endif // TCPPACKET_H
