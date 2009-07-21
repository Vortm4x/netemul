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
    all->addLayout(lay);
    setLayout(all);
}

void computerProperty::setDevice(computerSetting *c)
{
    comp = c;
    gateway->setText( c->gateway() );
    cb_route->setChecked( comp->isRouter() );
    btn_apply->setEnabled(false);
    te_text->setPlainText( comp->note() );
}

void computerProperty::apply()
{
    if (gateway->text() != "0.0.0.0" )
    comp->setGateway(gateway->text());
    comp->setRouter( cb_route->isChecked() );
    comp->setNote( te_text->toPlainText() );
    if ( sender() == btn_ok ) accept();
}

