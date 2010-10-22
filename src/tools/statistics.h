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

class Statistics
{    
public:
    enum { ReceiveFrames = 0 , SendFrames = 1 , ReceivePackets = 2 , SendPackets = 3 };
    Statistics();
    ~Statistics() { }
    Statistics(const Statistics &other);
    Statistics operator=(const Statistics &other);
    Statistics operator+(const Statistics &other);
    Statistics operator+=(const Statistics &other);
    QString toString() const;    
    void clear();

    quint64 receiveFrames;
    quint64 sendFrames;
    quint64 sendPackets;
    quint64 receivePackets;
protected:
    friend QDataStream& operator<<(QDataStream &stream, const Statistics &st);
    friend QDataStream& operator>>(QDataStream &stream, Statistics &st);
};

class StatisticsObject : public QObject
{
    Q_OBJECT
    Q_PROPERTY(quint64 receiveFrames READ receiveFrames WRITE setReceiveFrames)
    Q_PROPERTY(quint64 receivePackets READ receivePackets WRITE setReceivePackets)
    Q_PROPERTY(quint64 sendFrames READ sendFrames WRITE setSendFrames )
    Q_PROPERTY(quint64 sendPackets READ sendPackets WRITE setSendPackets )
public:
    StatisticsObject(Statistics s) { st = s; qDebug("Hello"); }
    StatisticsObject(QObject *parent = 0 ) : QObject(parent) { }    
    Statistics statistics() const { return st; }
    void setReceiveFrames(quint64 f) { st.receiveFrames = f; }
    void setSendFrames(quint64 f) { st.sendFrames = f; }
    void setSendPackets(quint64 p) { st.sendPackets = p; }
    void setReceivePackets(quint64 p) { st.receivePackets = p; }
    quint64 receiveFrames() const { return st.receiveFrames; }
    quint64 sendFrames() const { return st.sendFrames; }
    quint64 sendPackets() const { return st.sendPackets; }
    quint64 receivePackets() const { return st.receivePackets; }
private:
    Statistics st;
};

inline QDataStream& operator<<(QDataStream &stream, const Statistics &st)
{
    stream << st.receiveFrames << st.sendFrames << st.receivePackets << st.sendPackets;
    return stream;
}

inline QDataStream& operator>>(QDataStream &stream, Statistics &st)
{
    stream >> st.receiveFrames >> st.sendFrames >> st.receivePackets >> st.sendPackets;
    return stream;
}

#endif // STATISTICS_H
