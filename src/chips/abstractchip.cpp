#include "abstractchip.h"

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
    myStatistics.clear();
}
//------------------------------------------------

#ifndef __TESTING__
void abstractChip::write(QDataStream &stream) const
{
    stream << myMac;
    stream << myIp;
    stream << myMask;
    stream << myStatistics;
}

void abstractChip::read(QDataStream &stream)
{
    stream >> myMac;
    stream >> myIp;
    stream >> myMask;
    stream >> myStatistics;
}
#endif

void abstractChip::checkReceive(frame &f)
{
    emit receiveData(f.toData());
    myStatistics.incReceiveFrames();
    if ( f.type() == frame::ip ) myStatistics.incReceivePackets();
}

void abstractChip::checkSend(frame &f)
{
    emit sendData(f.toData());
    myStatistics.incSendFrames();
    if ( f.type() == frame::ip ) myStatistics.incSendPackets();
}



