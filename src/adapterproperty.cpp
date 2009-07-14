#include "adapterproperty.h"
#include "smartdevice.h"
#include "ipedit.h"
#include <QTabBar>
#include <QLabel>
#include <QLineEdit>
#include <QCheckBox>

/**
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

    lb_recFrame = new QLabel;
    lb_recPacket = new QLabel;
    lb_sendFrame = new QLabel;
    lb_sendPacket = new QLabel;
    connect(le_mask, SIGNAL(textChanged(QString)), SLOT(applyEnable()));

    all->addWidget(lb_recFrame);
    all->addWidget(lb_sendFrame);
    all->addWidget(lb_recPacket);
    all->addWidget(lb_sendPacket);

    all->addStretch(1);
    QPushButton *btn_reset = new QPushButton(QIcon(":/im/images/refresh.png"),trUtf8("Сбросить статистику"));
    connect( btn_reset , SIGNAL(clicked()) , SLOT(reset()));
    all->addWidget(btn_reset, 0 , Qt::AlignRight);
    all->addLayout(lay);
    setLayout(all);
    connect( le_ip , SIGNAL(maskChanged(quint8)) , le_mask , SLOT(setDefaultMask(quint8)));
    connect( tab_interfaces , SIGNAL(currentChanged(int)) , SLOT(changeTab()));
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
/**
  Задает диалогу устройство которое он будет отображать, так же эта функция, загружает первый адаптер устройства
  в диалог.
  @param r , указатель на устройво.
*/
void adapterProperty::setDevice(smartDevice *r)
{
    sd = r;
    QList<devicePort*> ports = sd->sockets();
    foreach ( devicePort *i , ports ) {
        tab_interfaces->addTab( i->connectIcon() , i->name() );
        if ( tab_interfaces->count() == 1 ) updateTab(i);
    }
    btn_apply->setEnabled(false);
}
//-----------------------------------------------------
void adapterProperty::changeTab()
{
    QList<devicePort*> ports = sd->sockets();
    foreach ( devicePort *i , ports ) {
        if ( i->name() == tab_interfaces->tabText(tab_interfaces->currentIndex()) ) {
            updateTab(i);
            break;
        }
    }
}
/**
  Обновляет содержимое диалога в зависимости от выбранного адапетра.
  @param d указатель на сокет выбранного интерфейса.
  */
void adapterProperty::updateTab(devicePort *d)
{
    interface *t = qobject_cast<interface*>(d->parentDev());
    le_name->setText(d->name());
    le_mac->setText( t->macString() );
    le_ip->setText( t->ip().ipString() );
    le_mask->setText( t->mask().ipString() );
    lb_recFrame->setText( trUtf8("Получено кадров: %1").arg( t->countRecFrame() ) );
    lb_recPacket->setText( trUtf8("Получено пакетов: %1").arg( t->countRecPacket() ) );
    lb_sendFrame->setText( trUtf8("Отправлено кадров: %1").arg( t->countSendFrame() ) );
    lb_sendPacket->setText( trUtf8("Отправлено пакетов: %1").arg( t->countSendPacket() ) );
    if (sd) sd->setCheckedSocket( d->name() );
}
//-----------------------------------------------------
void adapterProperty::apply()
{
    QList<devicePort*> ports = sd->sockets();
    foreach(devicePort *i, ports) {
        if (i->name() != tab_interfaces->tabText(tab_interfaces->currentIndex())) {
            if (le_name->text() == i->name()) {
                le_name->setText(tab_interfaces->tabText(tab_interfaces->currentIndex()));
                le_name->setSelection(0, le_name->text().length());
                break;
            }
        }
    }
    foreach ( devicePort *i , ports ) {
        if (tab_interfaces->tabText(tab_interfaces->currentIndex()) == i->name()) {
            i->setName(le_name->text());
            arpPacket *p = new arpPacket;
            frame *f = new frame;
            p->setReceiverIp(le_ip->text());
            p->setSenderIp(le_ip->text());
            p->setSenderMac(le_mac->text());
            p->setType(arpPacket::request);
            f->setReceiver(trUtf8("FF:FF:FF:FF:FF:FF"));
            f->setSender(le_mac->text());
            f->setType(frame::arp);
            *f << *p;
            f->setDifferent(frame::broadcast);
            i->sendFrame(f);
            i->parentDev()->setMac(le_mac->text());
            i->parentDev()->setIp(le_ip->text());
            i->parentDev()->setMask(le_mask->text());
            sd->connectedNet(i);
            break;
        }
    }
    if ( sender() == btn_ok ) accept();
}
//------------------------------------------------------------
/**
    Слот сбрасывает всю статистику у адаптера.
*/
void adapterProperty::reset()
{
    QString t = tab_interfaces->tabText( tab_interfaces->currentIndex() );
    sd->adapter(t)->resetStatics();
    updateTab(sd->socket(t));
}
//------------------------------------------------------------
