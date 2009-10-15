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
#include "ipedit.h"
#include <QLineEdit>
#include <QKeyEvent>
#include <QLabel>
#include <QIntValidator>
#include <QtDebug>
#include <QHBoxLayout>

/**
  Создает внешнее оформление виджета.
  @param str - строка отображаемая перед полем ввода.
  @param parent - виджет родитель.

  */
ipEdit::ipEdit(QString str, QWidget *parent /* = 0 */) : QWidget(parent)
{
    QIntValidator *v = new QIntValidator(1,255,this);
    QHBoxLayout *all = new QHBoxLayout;
    label = new QLabel(str);
    label->setFixedWidth(150);
    all->addWidget(label,0,Qt::AlignLeft);
    all->addStretch(1);
    part[0] = new QLineEdit(this);
    part[0]->setFixedSize(45,25);
    part[0]->setMaxLength(3);
    part[0]->setValidator(v);
    part[0]->installEventFilter(this);
//    setFixedSize( 450 , part[0]->height()+10 );  
    connect( part[0] , SIGNAL(textChanged(QString)), SIGNAL(textChanged(QString)));
    connect( part[0] , SIGNAL(textChanged(QString)), SLOT(changeMask(QString)));
    all->addWidget(part[0] );
    for ( int i = 1 ; i < 4; i++) {
        QLabel *p = new QLabel(".");
        p->setAlignment(Qt::AlignBottom | Qt::AlignHCenter);
        all->addWidget(p,0,Qt::AlignRight);
        part[i] = new QLineEdit(this);
        part[i]->setFixedSize(45,25);
        part[i]->setMaxLength(3);
        part[i]->setValidator(v);
        part[i]->installEventFilter(this);
        all->addWidget(part[i]);
        connect(part[i] , SIGNAL(textChanged(QString)), SIGNAL(textChanged(QString)));
    }
    clear();
    setLayout(all);
    setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
}

void ipEdit::setText(QString str)
{
    QStringList s = str.split(".");
    for (int i = 0 ; i < 4 ; i++)
        part[i]->setText(s.at(i));
}

QSize ipEdit::sizeHint() const
{
    return QSize(label->width() + part[0]->width() * 4 + 50, part[0]->height() +10 );
}

QString ipEdit::text() const
{
    QString s;
    s = part[0]->text();
    for ( int i = 1; i < 4 ; i++)
        s += QChar('.') + part[i]->text();
    return s;
}

/*!
  Сбрасывает значения всех полей в 0.
*/
void ipEdit::clear()
{
    for (int i = 0 ; i < 4 ; i++)
        part[i]->setText("0");
}
//---------------------------------------
/*!
  Исходя из первого байта ip адреса выставляет маску по умолчанию для этого класса сетей.
  @param u- первый байт ip адреса.
*/
void ipEdit::setDefaultMask(quint8 u)
{
    int n;
    if ( u >= 1 && u < 127 ) n = 1; // Далее это число
    else if ( u >= 127 && u < 192 ) n = 2; // Скажет нам к какому классу
    else if ( u >= 192 && u < 224 ) n = 3; // Сетей относиться адрес
    else n = 0;
    clear(); // Очистим
    for ( int i = 0 ; i < n ; i++ )
        part[i]->setText("255"); // И выствавим нужные нам в 255.
}
//----------------------------------------
/*!
  Слот посылает сигнал о том что изменился первый октет ip адреса
  сигнал может быть перехвачен другим ipEdit и использован для задания маски через
  слот setDefaultMask.
*/
void ipEdit::changeMask(QString s)
{
    quint8 u = s.toInt();
    emit maskChanged(u);
}
//-------------------------------------------
bool ipEdit::eventFilter(QObject *obj, QEvent *event)
{
    if ( event->type() == QEvent::KeyPress ) {
        QKeyEvent *e = static_cast<QKeyEvent*>(event);
        switch ( e->key() ) {
            case Qt::Key_Left : focusPreviousChild(); break;
            case Qt::Key_Right : focusNextChild(); break;
            case Qt::Key_Space : focusNextChild(); break;
            default: return QWidget::eventFilter(obj,event);
        }
        return true;
    }
    return QWidget::eventFilter(obj,event);
}

void ipEdit::setLabelText(const QString text)
{
    label->setText(text);
}

QString ipEdit::labelText() const
{
    return label->text();
}




