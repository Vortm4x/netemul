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
#ifndef IPEDIT_H
#define IPEDIT_H

#include <QWidget>
#include "ipaddress.h"

class QLineEdit;
class QLabel;
/**
  Виджет для ввода ip адреса.
*/
class ipEdit : public QWidget
{
    Q_OBJECT
    Q_PROPERTY( QString labelText READ labelText WRITE setLabelText DESIGNABLE true)
public:                
    ipEdit(QWidget *parent = 0, QString str = "");
    void setText(QString str);
    IpAddress ipText() const { return IpAddress(text()); }
    QString text() const;
    void setLabelText(const QString text);
    QString labelText() const;
    void clear();   
    QSize sizeHint() const;
    void setLabelVisible(bool isVisible);
public slots:
    void setDefaultMask(quint8 u);
    void changeMask(QString s);
private:
    QLabel *label;
    QLineEdit *part[4];
signals:
    void textChanged(QString);
    void maskChanged(quint8);
protected:
    bool eventFilter(QObject *obj, QEvent *event);
};
//-----------------------------------------
#endif // IPEDIT_H
