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

class Device;
class SmartDevice;
class Program;

typedef QList<Program*> ProgramList;

class Program : public QObject
{
    Q_OBJECT
    Q_PROPERTY( bool enable READ isEnable WRITE setEnable )
public:    
    Program(QObject *parent = 0);
    virtual ~Program();

    static Program* createFromStream(QObject *parent , QDataStream &stream);
    static Program* createImpl(QObject *parent , int n);

public:
    enum { RIP = 0 , DHCPClient = 1 , DHCPServer = 2, SPOOFING = 3 };

    void setEnable(bool b);
    bool isEnable() const  { return myEnable; }
    QString name() const { return myName; }
    virtual void setDevice(SmartDevice *s) { myDevice = s; }
    void updateView();
    SmartDevice* device() const { return myDevice; }
    virtual bool interrupt(int u) = 0;
    virtual void showProperty() = 0;
    virtual QString featureName() const = 0;
    virtual void incTime() { }
    virtual void write(QDataStream &stream) const;
    virtual void read(QDataStream &stream);
    virtual int id() const = 0;
protected:
    SmartDevice *myDevice;
    bool myEnable;
    QString myName; //!< Имя программы.
};

#endif // PROGRAMMREP_H
