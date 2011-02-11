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
#include "program.h"
#include "smartdevice.h"
#include "ripprogram.h"
#include "dhcpserverprogram.h"
#include "dhcpclientprogram.h"
#include "spoofingprogram.h"

static const int MAGIC_PROGRAMM_NUMBER = 50;

Program::Program(QObject *parent) : QObject(parent)
{
}

Program::~Program()
{    
}

void Program::setEnable(bool b)
{
    if ( b != myEnable ) {
        myEnable = b;        
    }
}

void Program::updateView()
{
    myDevice->updateView();
}

Program* Program::createFromStream(QObject *parent, QDataStream &stream)
{
    int n;
    stream >> n;
    Program *p = createImpl(parent,n);
    p->read(stream);
    return p;
}

Program* Program::createImpl(QObject *parent, int n)
{
    switch (n%MAGIC_PROGRAMM_NUMBER) {
        case RIP: return new RipProgram(parent);
        case DHCPClient : return new DhcpClientProgram(parent);
        case DHCPServer : return new DhcpServerProgram(parent);
        case SPOOFING : return new SpoofingProgram(parent);
        default: break;
    }
    return 0;
}

/*!
  Записывает программу в поток.
  @param stream - поток для записи.
*/
void Program::write(QDataStream &stream) const
{
    stream << myEnable;
}
//--------------------------------------------
/*!
  Считывает программу из потока.
  @param stream - поток для чтения.
*/
void Program::read(QDataStream &stream)
{
    stream >> myEnable;
}
//-------------------------------------------
