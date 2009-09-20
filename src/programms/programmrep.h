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
#ifndef PROGRAMMREP_H
#define PROGRAMMREP_H

#include <QDataStream>

class device;
class smartDevice;

class programmRep : public QObject
{
    Q_OBJECT
public:
    friend class programm;
    programmRep();
    virtual ~programmRep();
    void setEnable(bool b) { if ( b != myEnable ) myEnable = b; }
    bool isEnable() const  { return myEnable; }
    QString name() const { return myName; }
    virtual void setDevice(smartDevice *s) { device = s; }
    virtual bool interrupt(int u) = 0;
    virtual void showProperty() = 0;
    virtual void incTime() { }
    virtual void write(QDataStream &stream) const;
    virtual void read(QDataStream &stream);
protected:
    smartDevice *device;
    QString myName; //!< Имя программы.
private:
    quint8 countRef;
    bool myEnable;
};

#endif // PROGRAMMREP_H
