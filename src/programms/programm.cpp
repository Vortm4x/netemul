#include "programm.h"
#include "ripprogramm.h"
#include <QtDebug>

programm::programm(int n)
{
    qDebug() << "Create programm!";
    switch ( n ) {
        case RIP: rep = new ripProgramm; break;
        default: break;
    }
    rep->countRef = 1;
}

programm::programm(QDataStream &stream)
{
    qDebug() << "Create programm!";
    int n;
    stream >> n;
    switch (n) {
        case RIP: rep = new ripProgramm; break;
        default: break;
    }
    rep->read(stream);
    rep->countRef = 1;
}

programm::programm(const programm &other)
{
    rep = other.rep;
    rep->countRef++;
}

programm::~programm()
{
    if ( --rep->countRef <= 0 ) {
        qDebug() << "Destroy programm!";
        delete rep;
    }
}

programm& programm::operator=(const programm &other)
{
    other.rep->countRef++;
    if ( --rep->countRef <= 0 ) delete rep;
    rep = other.rep;
    return *this;
}


