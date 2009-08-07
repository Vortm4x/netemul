#include "adapterproperty.h"
#include "smartdevice.h"
#include "ipedit.h"
#include "interfacedialog.h"
#include <QTabBar>
#include <QLabel>
#include <QLineEdit>
#include <QCheckBox>
#include <QMessageBox>

/*!
  Конструктор создает основной интерфейс диалога.
*/
adapterProperty::adapterProperty(adapterSetting *s)
{
    sd = s;
    QVBoxLayout *all = new QVBoxLayout;
    tab_interfaces = new QTabBar;
    for ( int i = 0 ; i < sd->socketsCount() ; i++ ) {
        sd->setCurrent(i);
        tab_interfaces->addTab(  connectIcon(sd->isConnect()), sd->name() );
    }
    all->addWidget(tab_interfaces);

    QHBoxLayout *temp = new QHBoxLayout;
    QLabel *lbl_temp = new QLabel(tr("Netcard name: "));
    le_name = new QLineEdit;
    le_name->setMaxLength(10);
    le_name->setReadOnly(true);
    temp->addWidget(lbl_temp);
    temp->addWidget(le_name);
    all->addLayout(temp);
    connect(le_name, SIGNAL(textChanged(QString)), SLOT(applyEnable()));

    temp = new QHBoxLayout;
    lbl_temp = new QLabel(tr("Mac-address: "));
    le_mac = new QLineEdit;
    le_mac->setInputMask("HH:HH:HH:HH:HH:HH;_");
    temp->addWidget(lbl_temp);
    temp->addWidget(le_mac);
    all->addLayout(temp);
    connect(le_mac, SIGNAL(textChanged(QString)), SLOT(applyEnable()));

    le_ip = new ipEdit(tr("Ip-address: "));
    all->addWidget(le_ip);
    connect(le_ip, SIGNAL(textChanged(QString)), SLOT(applyEnable()));

    le_mask = new ipEdit(tr("Mask: "));
    all->addWidget(le_mask);

    lb_statics = new QLabel;
    connect(le_mask, SIGNAL(textChanged(QString)), SLOT(applyEnable()));

    all->addWidget(lb_statics);

    all->addStretch(1);
    QHBoxLayout *b = new QHBoxLayout;
    b->addStretch(1);
    if ( sd->canManageInterface() ) {
        btn_add = new QPushButton(QIcon(":/im/images/edit_add.png") , tr("Add") );
        btn_del = new QPushButton(QIcon(":/im/images/edit_remove.png") , tr("Delete") );
        connect( btn_add , SIGNAL(clicked()) , SLOT(addInterface()) );
        connect( btn_del , SIGNAL(clicked()) , SLOT(deleteInterface()) );
        b->addWidget(btn_add, 0 , Qt::AlignRight);
        b->addWidget(btn_del, 0 , Qt::AlignRight);
    }
    QPushButton *btn_reset = new QPushButton(QIcon(":/im/images/refresh.png"),tr("Reset statistics"));
    connect( btn_reset , SIGNAL(clicked()) , SLOT(reset()));
    b->addWidget(btn_reset, 0 , Qt::AlignRight);
    all->addLayout(b);
    all->addLayout(lay);
    setLayout(all);
    connect( le_ip , SIGNAL(maskChanged(quint8)) , le_mask , SLOT(setDefaultMask(quint8)));
    connect( tab_interfaces , SIGNAL(currentChanged(int)) , SLOT(changeTab(int)));
    btn_apply->setEnabled(false);
    updateTab(0);
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
    else return QIcon(":/im/images/not.png");
}

void adapterProperty::addInterface()
{
    interfaceDialog *d = new interfaceDialog;
    if ( d->exec() ) {
        //int n = d->result();
        sd->addInterface();
        sd->setCurrent( tab_interfaces->count() );
        tab_interfaces->addTab(  connectIcon(sd->isConnect()), sd->name() );
        updateTab( tab_interfaces->count() - 1 );
    }
    delete d;
}

void adapterProperty::deleteInterface()
{
    if ( sd->isConnect() ) {
        QMessageBox::warning(this,tr("Error") ,
                             tr("First, remove the cable!") , QMessageBox::Ok,
                             QMessageBox::Ok );
        return;
    }

    if ( tab_interfaces->count() == 1 ) {
        QMessageBox::warning(this,tr("Error") , tr("Must be at least one netcard") ,
                             QMessageBox::Ok , QMessageBox::Ok );
        return;
    }

    tab_interfaces->removeTab( sd->current() );
    sd->deleteInterface( le_name->text() );
    tab_interfaces->setCurrentIndex(0);
    updateTab(0);
}
