#include "adapterproperty.h"
#include "smartdevice.h"
#include "ipedit.h"
#include <QTabBar>
#include <QLabel>
#include <QLineEdit>
#include <QCheckBox>

/*!
  Конструктор создает основной интерфейс диалога.
*/
adapterProperty::adapterProperty()
{
    sd = NULL;
    QVBoxLayout *all = new QVBoxLayout;
    tab_interfaces = new QTabBar;
    all->addWidget(tab_interfaces);

    QHBoxLayout *temp = new QHBoxLayout;
    QLabel *lbl_temp = new QLabel(trUtf8("Имя адаптера: "));
    le_name = new QLineEdit;
    le_name->setMaxLength(10);
    temp->addWidget(lbl_temp);
    temp->addWidget(le_name);
    all->addLayout(temp);
    connect(le_name, SIGNAL(textChanged(QString)), SLOT(applyEnable()));

    temp = new QHBoxLayout;
    lbl_temp = new QLabel(trUtf8("Mac-адресс: "));
    le_mac = new QLineEdit;
    le_mac->setInputMask("HH:HH:HH:HH:HH:HH;_");
    temp->addWidget(lbl_temp);
    temp->addWidget(le_mac);
    all->addLayout(temp);
    connect(le_mac, SIGNAL(textChanged(QString)), SLOT(applyEnable()));

    le_ip = new ipEdit(trUtf8("Ip-адресс: "));
    all->addWidget(le_ip);
    connect(le_ip, SIGNAL(textChanged(QString)), SLOT(applyEnable()));

    le_mask = new ipEdit(trUtf8("Маска подсети: "));
    all->addWidget(le_mask);

    lb_statics = new QLabel;
    connect(le_mask, SIGNAL(textChanged(QString)), SLOT(applyEnable()));

    all->addWidget(lb_statics);

    all->addStretch(1);
    QPushButton *btn_reset = new QPushButton(QIcon(":/im/images/refresh.png"),trUtf8("Сбросить статистику"));
    connect( btn_reset , SIGNAL(clicked()) , SLOT(reset()));
    all->addWidget(btn_reset, 0 , Qt::AlignRight);
    all->addLayout(lay);
    setLayout(all);
    connect( le_ip , SIGNAL(maskChanged(quint8)) , le_mask , SLOT(setDefaultMask(quint8)));
    connect( tab_interfaces , SIGNAL(currentChanged(int)) , SLOT(changeTab(int)));
}
//-----------------------------------------------------------
/*!
  Деструктор снимает выделение определенного интерфейса у устройства, тем самым когда мы
  выходим из дилога у нас нет выделенного кабеля.
*/
adapterProperty::~adapterProperty()
{
    sd->setCheckedSocket("");
}
//--------------------------------------------------
/*!
  Задает диалогу настройки которое он будет отображать.
  @param s указатель на настройки.
*/
void adapterProperty::setDevice(adapterSetting *s)
{
    sd = s;
    for ( int i = 0 ; i < sd->socketsCount() ; i++ ) {
        sd->setCurrent(i);
        tab_interfaces->addTab(  connectIcon(sd->isConnect()), sd->name() );
    }
    updateTab(0);
    btn_apply->setEnabled(false);
}
//-----------------------------------------------------
/*!
  При смене вкладки вызывает обновление содержимого.
  @param n - Номер выбранной вкладки.
*/
void adapterProperty::changeTab(int n)
{
    updateTab(n);
}
//--------------------------------------------------------
/*!
  Обновляет содержимое диалога в зависимости от выбранного адапетра.
  @param d указатель на сокет выбранного интерфейса.
*/
void adapterProperty::updateTab(int n)
{
    sd->setCurrent(n);
    le_name->setText( sd->name() );
    le_mac->setText( sd->mac() );
    le_ip->setText( sd->ip() );
    le_mask->setText( sd->mask() );
    lb_statics->setText( sd->statics() );
    if (sd) sd->setCheckedSocket( sd->name() );
}
//-----------------------------------------------------
void adapterProperty::apply()
{
    for ( int i = 0 ; i < tab_interfaces->count(); i++ )
        if ( i != tab_interfaces->currentIndex() && le_name->text() == tab_interfaces->tabText(i) ) {
            le_name->setText(tab_interfaces->tabText(tab_interfaces->currentIndex()));
            le_name->setSelection(0, le_name->text().length());
            break;
        }
    sd->setName( le_name->text() );
    sd->setMac( le_mac->text() );
    sd->setIp( le_ip->text() );
    sd->setMask( le_mask->text() );
    sd->connectedNet();
    sd->sendArpRequest(le_ip->text());
    if ( sender() == btn_ok ) accept();
}
//------------------------------------------------------------
/*!
    Слот сбрасывает всю статистику у адаптера.
*/
void adapterProperty::reset()
{
    sd->resetStatics();
    updateTab(tab_interfaces->currentIndex());
}
//------------------------------------------------------------
QIcon adapterProperty::connectIcon(bool b)
{
    if ( b) return QIcon(":/im/images/ok.png");
    else return QIcon(":/im/images/minus2.png");
}
