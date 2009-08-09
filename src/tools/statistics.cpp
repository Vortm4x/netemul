#include "statistics.h"

statistics::statistics()
{
    clear();
}

void statistics::clear()
{
    myReceiveFrames = 0 ;
    myReceivePackets = 0 ;
    mySendFrames = 0;
    mySendPackets = 0;
}

statistics::statistics(const statistics &other)
{
    myReceiveFrames = other.myReceiveFrames ;
    myReceivePackets = other.myReceivePackets ;
    mySendFrames = other.mySendFrames ;
    mySendPackets = other.mySendPackets ;
}

statistics statistics::operator=(const statistics &other)
{
    myReceiveFrames = other.myReceiveFrames ;
    myReceivePackets = other.myReceivePackets ;
    mySendFrames = other.mySendFrames ;
    mySendPackets = other.mySendPackets ;
    return *this;
}

statistics statistics::operator+(const statistics &other)
{
    statistics temp;
    temp.myReceiveFrames = myReceiveFrames + other.myReceiveFrames ;
    temp.myReceivePackets = myReceivePackets + other.myReceivePackets ;
    temp.mySendFrames = mySendFrames + other.mySendFrames ;
    temp.mySendPackets = mySendPackets + other.mySendPackets ;
    return temp;
}

statistics statistics::operator+=(const statistics &other)
{
    *this = *this + other;
    return *this;
}

QString statistics::toString() const
{
    QString t;
    t += QObject::tr("Received frames: %1\n").arg( myReceiveFrames ) +
    QObject::tr("Received packets: %1\n").arg( myReceivePackets ) +
    QObject::tr("Sent frames: %1\n").arg( mySendFrames ) +
    QObject::tr("Sent packets: %1\n").arg( mySendPackets ) ;
    return t;
}

