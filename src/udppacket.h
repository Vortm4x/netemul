#ifndef UDPPACKET_H
#define UDPPACKET_H

#include <QDataStream>
#include <QSharedData>

class udpPacketData : public QSharedData
{
public:
    udpPacketData() { }
    udpPacketData(const udpPacketData &other);
    ~udpPacketData() { }
    QByteArray toData() const;
    friend class udpPacket;
private:
    quint16 sender; //!< Порт отправителя
    quint16 receiver; //!< Порт получателя
    QByteArray data; //!< Поле данных
};
/*!
  Реализует udp-дейтаграмму
*/
class udpPacket
{
public:
    enum { User = 7777 , RIP = 520 , DHCPClient = 67, DHCPServer = 68 } ;
    udpPacket() { d = new udpPacketData; }
    udpPacket(const QByteArray &b);
    udpPacket(const udpPacket &u) : d(u.d) { }
    ~udpPacket() { }
    QByteArray toData() const;
    int size() const { return d->data.size(); }
    void setSender( quint16 i ) { d->sender = i;}
    void setReceiver( quint16 i ) { d->receiver = i; }
    quint16 sender() const { return d->sender; }
    quint16 receiver() const { return d->receiver; }
    void pack(const QByteArray &b) { d->data = b; }
    QByteArray unpack() const { return d->data; }
    QString typeToString() const;
    QString toString() const;
private:
    QSharedDataPointer<udpPacketData> d;
protected:
    friend QDataStream& operator<<( QDataStream &stream, const udpPacket &p );
};
//-----------------------------------------------------
/*!
  Записывает udp-дейтаграмму в поток.
  @param stream - поток для записи.
  @param p - записываемый пакет.
  @return ссылку на результирующий поток.
*/
inline QDataStream& operator<<( QDataStream &stream, const udpPacket &p )
{
    stream << p.toData();
    return stream;
}
//-------------------------------------------------------
#endif // UDPPACKET_H
