#include "routerproperty.h"
#include "routerdevice.h"
#include <QCheckBox>
#include <QComboBox>
#include <QGroupBox>
#include <QLabel>

/*!
  Создает интерфейс диалога.
*/
routerProperty::routerProperty()
{
    QVBoxLayout *all = new QVBoxLayout;
    QHBoxLayout *temp = new QHBoxLayout;
    temp->addWidget( new QLabel(trUtf8("Количество портов: ") ) );
    cb_count = new QComboBox;
    QStringList t;
    t << "2" << "4" << "5"<< "7" << "8" << "9";
    cb_count->addItems(t);
    temp->addWidget(cb_count);
    all->addLayout(temp);
    cb_route = new QCheckBox( trUtf8("Включить маршрутизацию.") );
    connect( cb_route , SIGNAL(toggled(bool)) , SLOT(applyEnable()) );
    all->addWidget( cb_route);
    all->addStretch(1);
    temp = new QHBoxLayout;
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
//----------------------------------------------------------------
/*!
 Задает диалогу устройство для работы.
 @param r - указатель на роутер.
*/
void routerProperty::setRouter(routerDevice *r)
{
    rt = r;
    cb_route->setChecked(r->routeMode());
    cb_count->setCurrentIndex( cb_count->findText( QString::number( rt->sockets().count() ) ) );
    btn_apply->setEnabled(false);
}
//-----------------------------------------------------------------
void routerProperty::apply()
{
    rt->setRouteMode( cb_route->isChecked() );
    if ( sender() == btn_ok ) accept();
}

void routerProperty::tableDialog()
{
    rt->showDialog<routeEditor>();
}

void routerProperty::adapterDialog()
{
    rt->showDialog<adapterProperty>();
}

void routerProperty::arpDialog()
{
    rt->showDialog<tableArp>();
}


