#ifndef PROGRAMM_H
#define PROGRAMM_H

#include <QDataStream>
#include "programmrep.h"

class programm
{
public:
    enum { RIP = 50 };
    programm() { qFatal("Error programm constructor!"); }
    programm(int n);
    programm(QDataStream &stream);
    programm(const programm &other);
    ~programm();
    programm& operator=(const programm &other);
    bool operator==(const programm &other) { return rep == other.rep; }
    programmRep* operator->() const { return rep; }
private:
    programmRep *rep;
protected:
    friend QDataStream& operator<<(QDataStream &stream,const programm &p);
    friend QDataStream& operator>>(QDataStream &stream,programm &p);
};
/*!
  Записывает программу в поток.
*/
inline QDataStream& operator<<(QDataStream &stream,const programm &p)
{
    p.rep->write(stream);
    return stream;
}
//--------------------------------------------------------------------

#endif // PROGRAMM_H
