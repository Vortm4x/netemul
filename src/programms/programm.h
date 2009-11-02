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
#ifndef PROGRAMM_H
#define PROGRAMM_H

#include <QDataStream>
#include "programmrep.h"

class programm
{
public:
    enum { RIP = 0 , DHCPClient = 1 , DHCPServer = 2 };
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
