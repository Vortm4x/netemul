#include "adapterproperty.h"
#include "smartdevice.h"
#include "ipedit.h"
#include <QTabBar>
#include <QLabel>
#include <QLineEdit>
#include <QCheckBox>

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
;
    all->addLayout(lay);
    setLayout(all);

    connect( tab_interfaces , SIGNAL(currentChanged(int)) , SLOT(changeTab()));
}

void adapterProperty::setSmart(smartDevice *r)
{
    sd = r;
    QList<devicePort*> ports = sd->sockets();
    foreach ( devicePort *i , ports ) {
        tab_interfaces->addTab( i->connectIcon() , i->name() );
        if ( tab_interfaces->count() == 1 ) updateTab(i);
    }
    btn_apply->setEnabled(false);
}

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

void adapterProperty::updateTab(devicePort *d)
{
    le_name->setText(d->name());
    le_mac->setText( d->parentDev()->macString() );
    le_ip->setText( d->parentDev()->ip().ipString() );
    le_mask->setText( d->parentDev()->mask().ipString() );
    lb_recFrame->setText( trUtf8("Получено кадров: %1").arg( d->parentDev()->countRecFrame() ) );
    lb_recPacket->setText( trUtf8("Получено пакетов: %1").arg( d->parentDev()->countRecPacket() ) );
    lb_sendFrame->setText( trUtf8("Отправлено кадров: %1").arg( d->parentDev()->countSendFrame() ) );
    lb_sendPacket->setText( trUtf8("Отправлено пакетов: %1").arg( d->parentDev()->countSendPacket() ) );
    if (sd) sd->setCheckedSocket( d->name() );
}

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
            i->parentDev()->setMac(le_mac->text());
            i->parentDev()->setIp(le_ip->text());
            i->parentDev()->setMask(le_mask->text());
            sd->connectedNet(i);
            break;
        }
    }
    if ( sender() == btn_ok ) accept();
}
