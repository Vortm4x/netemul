/****************************************************************************************
** NetEmul - program for simulating computer networks.
** Copyright © 2009 Semenov Pavel and Omilaeva Anastasia
**
** NetEmul is free software; you can redistribute it and/or
** modify it under the terms of the GNU Lesser General Public
** License as published by the Free Software Foundation; either
** version 2.1 of the License, or (at your option) any later version.
**
** NetEmul is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
** Lesser General Public License for more details.
**
** You should have received a copy of the GNU Lesser General Public
** License along with the NetEmul; if not, write to the Free
** Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
** 02111-1307 USA.
****************************************************************************************/
#ifndef STATISTICS_H
#define STATISTICS_H

#include <QtCore/QDataStream>
#include <QtCore/QXmlStreamWriter>
#include <QtCore/QXmlStreamReader>

class statistics
{
public:
    enum { ReceiveFrames = 0 , SendFrames = 1 , ReceivePackets = 2 , SendPackets = 3 };
    statistics();
    ~statistics() { }
    statistics(const statistics &other);
    statistics operator=(const statistics &other);
    statistics operator+(const statistics &other);
    statistics operator+=(const statistics &other);
    QString toString() const;
    void clear();
    quint64 receiveFrames() { return myReceiveFrames; }
    quint64 sendFrames() { return mySendFrames; }
    quint64 sendPackets() { return mySendPackets; }
    quint64 receivePackets() { return myReceivePackets; }
    void incReceiveFrames() { myReceiveFrames++; }
    void incSendFrames() { mySendFrames++; }
    void incSendPackets() { mySendPackets++; }
    void incReceivePackets() { myReceivePackets++; }
    void writeXml(QXmlStreamWriter &stream) const;
    void readXml(QXmlStreamReader &stream);
private:
    quint64 myReceiveFrames;
    quint64 mySendFrames;
    quint64 mySendPackets;
    quint64 myReceivePackets;
protected:
    friend QDataStream& operator<<(QDataStream &stream, const statistics &st);
    friend QDataStream& operator>>(QDataStream &stream, statistics &st);
};

inline QDataStream& operator<<(QDataStream &stream, const statistics &st)
{
    stream << st.myReceiveFrames << st.mySendFrames << st.myReceivePackets << st.mySendPackets;
    return stream;
}

inline QDataStream& operator>>(QDataStream &stream, statistics &st)
{
    stream >> st.myReceiveFrames >> st.mySendFrames >> st.myReceivePackets >> st.mySendPackets;
    return stream;
}

#endif // STATISTICS_H
