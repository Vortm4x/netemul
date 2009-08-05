#include "programm.h"
#include "ripprogramm.h"

programm::programm(int n)
{
    switch ( n ) {
        case RIP: rep = new ripProgramm; break;
        default: break;
    }
    rep->countRef = 1;
}

programm::programm(QDataStream &stream)
{
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


