#include "routerproperty.h"
#include "routerdevice.h"
#include "routeeditor.h"
#include "adapterproperty.h"
#include <QRadioButton>
#include <QGroupBox>

routerProperty::routerProperty()
{
    QVBoxLayout *all = new QVBoxLayout;
    QVBoxLayout *box = new QVBoxLayout;
    createButtons();
    for ( int i = 0 ; i < 3 ; i++) {
        box->addWidget(rtn_route[i]);
        connect( rtn_route[i] , SIGNAL(toggled(bool)) , SLOT(applyEnable()) );
    }
    box_group = new QGroupBox(trUtf8("Режим маршрутизации:"));
    box_group->setLayout(box);
    all->addWidget(box_group);
    all->addStretch(1);
    QHBoxLayout *temp = new QHBoxLayout;
    btn_adapter = new QPushButton(trUtf8("Интерфейсы"));
    connect(btn_adapter, SIGNAL(clicked()), SLOT(adapterDialog()));
    btn_table = new QPushButton(trUtf8("Таблица маршрутизации"));
    connect(btn_table, SIGNAL(clicked()), SLOT(tableDialog()));
    btn_arp = new QPushButton(trUtf8("Arp-таблица"));
    connect(btn_arp, SIGNAL(clicked()), SLOT(arpDialog()));
    temp->addWidget(btn_adapter);
    temp->addWidget(btn_table);
    temp->addWidget(btn_arp);
    temp->setAlignment(Qt::AlignRight);
    all->addLayout(temp);
    all->addLayout(lay);
    setLayout(all);
}

void routerProperty::setRouter(routerDevice *r)
{
    rt = r;
    rtn_route[r->routeMode()]->setChecked(true);
    btn_apply->setEnabled(false);
}

void routerProperty::apply()
{
    for ( int i = 0 ; i < 3 ; i++ )
        if ( rtn_route[i]->isChecked() ) rt->setRouteMode(i);
    if ( sender() == btn_ok ) accept();
}

void routerProperty::tableDialog()
{
    routeEditor *d = new routeEditor;
    d->setDevice(rt);
    d->exec();
    delete d;
}

void routerProperty::adapterDialog()
{
    adapterProperty *d = new adapterProperty;
    d->setSmart(rt);
    d->exec();
    delete d;
}

void routerProperty::arpDialog()
{
    rt->arpShow();
}

void routerProperty::createButtons()
{
    QStringList l;
    l << trUtf8("Маршрутизация отключена") << trUtf8("Статическая маршрутизация") << trUtf8("Протокол RIP") ;
    for ( int i = 0; i < 3 ; i++ )
        rtn_route[i] = new QRadioButton(l.at(i));
}
