#include "switchproperty.h"
#include "switchdevice.h"
#include "ipedit.h"
#include <QLabel>
#include <QCheckBox>
#include <QComboBox>
#include <QLineEdit>
#include <QMessageBox>
#include <QPlainTextEdit>


switchProperty::switchProperty()
{
    QVBoxLayout *all = new QVBoxLayout;
    QHBoxLayout *temp = new QHBoxLayout;
    QLabel *t = new QLabel(trUtf8("Количество портов: "));
    cb_count = new QComboBox;
    QStringList s;
    s << "4" << "5" << "8" << "12" <<"16"<< "24"<<"32" << "48";
    cb_count->addItems(s);
    connect( cb_count , SIGNAL(currentIndexChanged(int)) ,SLOT(applyEnable()) );
    temp->addWidget(t);
    temp->addWidget(cb_count);
    all->addLayout(temp);
    chk_manual = new QCheckBox(trUtf8("Управление через SNMP: "));
    connect( chk_manual , SIGNAL(clicked(bool)) , SLOT(check(bool)));
    all->addWidget(chk_manual);

    temp = new QHBoxLayout;
    lb_mac = new QLabel(trUtf8("Mac-адрес: "));
    le_mac = new QLineEdit;
    le_mac->setInputMask("HH:HH:HH:HH:HH:HH;_");
    temp->addWidget(lb_mac);
    temp->addWidget(le_mac);
    all->addLayout(temp);
    connect( le_mac , SIGNAL(textChanged(QString)) , SLOT(applyEnable()));

    le_ip = new ipEdit(trUtf8("Ip-адрес: "));
    all->addWidget(le_ip);
    connect( le_ip , SIGNAL(textChanged(QString)) , SLOT(applyEnable()) );

    le_mask = new ipEdit(trUtf8("Маска подсети: "));
    all->addWidget(le_mask);
    connect( le_mask , SIGNAL(textChanged(QString)) , SLOT(applyEnable()));

    lb_recFrame = new QLabel;
    lb_sendFrame = new QLabel;
    lb_recPacket = new QLabel;
    lb_sendPacket = new QLabel;
    all->addWidget(lb_recFrame);
    all->addWidget(lb_sendFrame);
    all->addWidget(lb_recPacket);
    all->addWidget(lb_sendPacket);
    all->addStretch(1);
    te_text = new QPlainTextEdit;
    connect( te_text , SIGNAL(textChanged()) , SLOT(applyEnable()) );
    te_text->setFixedHeight(100);
    te_text->setMaximumBlockCount(5);
    all->addWidget( new QLabel(trUtf8("Пояснения:")));
    all->addWidget(te_text);
    all->addStretch(1);
    all->addLayout(lay);
    setLayout(all);
}

void switchProperty::setSwitch(switchDevice *d)
{
    sw = d;
    QString c = QString().setNum( d->sockets().count() );
    cb_count->setCurrentIndex( cb_count->findText(c) );
    le_mac->setText( d->macString() );
    le_ip->setText( d->ip().ipString() );
    le_mask->setText( d->mask().ipString() );
    lb_recFrame->setText( trUtf8("Получено кадров: %1").arg( d->countRecFrame() ) );
    lb_recPacket->setText( trUtf8("Получено пакетов: %1").arg( d->countRecPacket() ) );
    lb_sendFrame->setText( trUtf8("Отправлено кадров: %1").arg( d->countSendFrame() ) );
    lb_sendPacket->setText( trUtf8("Отправлено пакетов: %1").arg( d->countSendPacket() ) );
    chk_manual->setChecked( d->isManual() );
    check( d->isManual() );
    te_text->setPlainText( sw->toolTip() );
    btn_apply->setEnabled(false);
}

void switchProperty::apply()
{
    int t = sw->sockets().count();
    if ( sw->sockets().count() != cb_count->currentText().toInt() )
        if ( !sw->setSocketCount( cb_count->currentText().toInt() ) ) {
            QMessageBox::warning(this,trUtf8("Error"),
            trUtf8("Для изменения количества портов отсоедините все провода.") , QMessageBox::Ok , QMessageBox::Ok);
            QString c = QString().setNum( t );
            cb_count->setCurrentIndex( cb_count->findText(c) );
            return;
        }
    sw->setManual( chk_manual->isChecked() );
    sw->setMac(le_mac->text());
    sw->setIp(le_ip->text());
    sw->setIp(le_mask->text());
    sw->setToolTip( te_text->toPlainText() );
    if ( sender() == btn_ok ) accept();
}

void switchProperty::check(bool p)
{
    lb_mac->setVisible(p);
    le_mac->setVisible(p);
    le_ip->setVisible(p);
    le_mask->setVisible(p);
    lb_recFrame->setVisible(p);
    lb_sendFrame->setVisible(p);
    lb_recPacket->setVisible(p);
    lb_sendPacket->setVisible(p);
    applyEnable();
}



