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
#ifndef LOGDIALOG_H
#define LOGDIALOG_H

#include "ui_logdialog.h"
#include "frame.h"

static const QColor cl_frame = "#fbec5d";
static const QColor cl_ipInternal = "#efaf8c";
static const QColor cl_udpInternal = "#ccccff";
static const QColor cl_rip = "#f78d9d";
static const QColor cl_user = "#badbad";
static const QColor cl_dhcp = "#f2e8c9" ;
static const QColor cl_undef = "#f984e5";
static const QColor cl_arp = "#7fc7ff";
static const QColor cl_arpInternal = "#c0c0c0";
static const QColor cl_tcpInternal = "#008080";
static const QColor cl_tcpData = "#c0c0c0";

class logDialog : public QWidget , private Ui::logDialog
{
    Q_OBJECT
    Q_DISABLE_COPY(logDialog)
public:
    logDialog(QStringList list);
    ~logDialog();
    enum { send = 0, receive = 1 };
    enum { all = 0 , arp = 1 , tcp = 2 , udp = 3 };
signals:
    void changeInterface(QString);
public  slots:
    void receiveData(frame fr,QString port);
    void sendData(frame fr,QString port);
protected:
    bool eventFilter(QObject *obj,QEvent *e);
    void changeEvent(QEvent *e);
private:
    void printRecord( int c, frame fr );
    QTreeWidgetItem* newItemWidget( QTreeWidgetItem *parent , const QString &text , const QColor color = "#FFFFFF");
    QString parseIp(frame fr,QTreeWidgetItem *parent);
    QString parseArp(frame fr,QTreeWidgetItem *parent);
};

#endif // LOGDIALOG_H
