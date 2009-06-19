#include "ipedit.h"
#include <QLineEdit>
#include <QKeyEvent>
#include <QLabel>
#include <QIntValidator>
#include <QtDebug>
#include <QHBoxLayout>

ipEdit::ipEdit(QString str, QWidget *parent /* = 0 */) : QWidget(parent)
{
    QIntValidator *v = new QIntValidator(1,255,this);
    QHBoxLayout *all = new QHBoxLayout;
    label = new QLabel(str);
    all->addWidget(label,0,Qt::AlignLeft);
    all->addStretch(1);
    part[0] = new QLineEdit(this);
    part[0]->setFixedSize(45,25);
    part[0]->setMaxLength(3);
    part[0]->setValidator(v);
    part[0]->setText("0");
    setFixedSize( 450 , part[0]->height()+10 );
    connect( part[0] , SIGNAL(textChanged(QString)), SIGNAL(textChanged(QString)));
    all->addWidget(part[0] , 0 , Qt::AlignRight);
    for ( int i = 1 ; i < 4; i++) {
        QLabel *p = new QLabel(".");
        p->setAlignment(Qt::AlignBottom | Qt::AlignHCenter);
        all->addWidget(p,0,Qt::AlignRight);
        part[i] = new QLineEdit(this);
        part[i]->setFixedSize(45,25);
        part[i]->setMaxLength(3);
        part[i]->setValidator(v);
        part[i]->setText("0");
        all->addWidget(part[i] , 0 , Qt::AlignRight);
        connect(part[i] , SIGNAL(textChanged(QString)), SIGNAL(textChanged(QString)));
    }
    setLayout(all);
}

void ipEdit::setText(QString str)
{
    QStringList s = str.split(".");
    for (int i = 0 ; i < 4 ; i++)
        part[i]->setText(s.at(i));
}

QString ipEdit::text() const
{
    QString s;
    s = part[0]->text();
    for ( int i = 1; i < 4 ; i++)
        s += QChar('.') + part[i]->text();
    return s;
}

ipAddress ipEdit::ipText() const
{
    return ipAddress(text());
}

void ipEdit::clear()
{
    for (int i = 0 ; i < 4 ; i++)
        part[i]->setText("0");
}



