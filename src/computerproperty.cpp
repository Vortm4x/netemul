#include "computerproperty.h"
#include "computer.h"
#include "ipedit.h"
#include <QCheckBox>
#include <QLabel>
#include <QPlainTextEdit>

computerProperty::computerProperty()
{
    QVBoxLayout *all = new QVBoxLayout;
    QVBoxLayout *prop = new QVBoxLayout;
    QHBoxLayout *temp = new QHBoxLayout;
    gateway = new ipEdit(trUtf8("Шлюз по умолчанию:"));
    connect( gateway , SIGNAL(textChanged(QString)) , SLOT(applyEnable()) );
    temp->addWidget(gateway,0,Qt::AlignLeft);
    prop->addLayout(temp);
    cb_route = new QCheckBox(trUtf8("Включить маршрутизацию"));
    connect( cb_route , SIGNAL(stateChanged(int)) , SLOT(applyEnable()) );
    prop->addWidget(cb_route);
    te_text = new QPlainTextEdit;
    connect( te_text , SIGNAL(textChanged()) , SLOT(applyEnable()) );
    te_text->setFixedHeight(100);
    te_text->setMaximumBlockCount(5);
    prop->addWidget( new QLabel(trUtf8("Пояснения:")));
    prop->addWidget(te_text);
    prop->addStretch(1);
    all->addLayout(prop);
    temp = new QHBoxLayout;
    btn_adapter = new QPushButton(trUtf8("Интерфейсы"));
    connect(btn_adapter, SIGNAL(clicked()), SLOT(adapterDialog()));
    btn_table = new QPushButton(trUtf8("Таблица маршрутизации"));
    connect(btn_table, SIGNAL(clicked()), SLOT(tableDialog()));
    btn_arp = new QPushButton(trUtf8("ARP-таблица"));
    connect(btn_arp, SIGNAL(clicked()), SLOT(arpDialog()));
    temp->addWidget(btn_adapter);
    temp->addWidget(btn_table);
    temp->addWidget(btn_arp);
    temp->setAlignment(Qt::AlignRight);
    all->addLayout(temp);
    all->addLayout(lay);
    setLayout(all);
}

void computerProperty::setComputer(computer *c)
{
    comp = c;
    gateway->setText(c->gateway().ipString());
    cb_route->setChecked( comp->routeMode() );
    btn_apply->setEnabled(false);
    cb_route->setEnabled( comp->sockets().count() > 1 );
    te_text->setPlainText( comp->toolTip() );
}

void computerProperty::apply()
{
    if (gateway->text() != "0.0.0.0" )
        comp->setGateway(gateway->text());
    comp->setRouteMode( cb_route->isChecked() );
    comp->setToolTip( te_text->toPlainText() );
    if ( sender() == btn_ok ) accept();
}

void computerProperty::tableDialog()
{
    comp->showDialog<routeEditor>();
}

void computerProperty::adapterDialog()
{
    comp->showDialog<adapterProperty>();
}

void computerProperty::arpDialog()
{
    comp->showDialog<tableArp>();
}

