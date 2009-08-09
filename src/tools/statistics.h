#ifndef STATISTICS_H
#define STATISTICS_H

#include <QtCore/QDataStream>

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
    quint64 sendPackets() { return myReceivePackets; }
    quint64 receivePackets() { return mySendPackets; }
    void incReceiveFrames() { myReceiveFrames++; }
    void incSendFrames() { mySendFrames++; }
    void incSendPackets() { myReceivePackets++; }
    void incReceivePackets() { mySendPackets++; }
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
