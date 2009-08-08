#include "abstractchip.h"
#include "frame.h"

/*!
  Задает устройству случайный Mac-адрес, пустой ip и маску подсети.
*/
abstractChip::abstractChip()
{
#ifndef __TESTING__
    myMac.setRandom();
#endif
    resetStatics();
}
//--------------------------------------------------------------
/*!
  Сбрасывает всю статистику, накопленную этим устройством.
*/
void abstractChip::resetStatics()
{
    myReceiveFrame = 0;
    myReceivePacket = 0;
    mySendFrame = 0 ;
    mySendPacket = 0;
}
//------------------------------------------------

QString abstractChip::staticsString() const
{
    QString t;
    t += QObject::tr("Receive frames: %1\n").arg( myReceiveFrame ) +
    QObject::tr("Receive packets: %1\n").arg( myReceivePacket ) +
    QObject::tr("Send frames: %1\n").arg( mySendFrame ) +
    QObject::tr("Send packets: %1\n").arg( mySendPacket ) ;
    return t;
}

#ifndef __TESTING__
void abstractChip::write(QDataStream &stream) const
{
    stream << myMac;
    stream << myIp;
    stream << myMask;
    stream << myReceiveFrame;
    stream << mySendFrame;
    stream << myReceivePacket;
    stream << mySendPacket;
}

void abstractChip::read(QDataStream &stream)
{
    stream >> myMac;
    stream >> myIp;
    stream >> myMask;
    stream >> myReceiveFrame;
    stream >> mySendFrame;
    stream >> myReceivePacket;
    stream >> mySendPacket;
}
#endif

void abstractChip::checkReceive(frame &f)
{
    myReceiveFrame++;
    if ( f.type() == frame::ip ) myReceivePacket++;
}

void abstractChip::checkSender(frame &f)
{
    mySendFrame++;
    if ( f.type() == frame::ip ) mySendPacket++;
}


