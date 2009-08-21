#include <QtCore/QTime>
#include "logdialog.h"
#include "frame.h"
#include "ippacket.h"
#include "arppacket.h"
#include "appsetting.h"
#include "udppacket.h"

logDialog::logDialog(QStringList list)
{
    setupUi(this);
    cb_sockets->addItems(list);
    startTimer(10*appSetting::animateSpeed());
    connect( cb_sockets ,SIGNAL(currentIndexChanged(QString)) , this , SIGNAL(changeInterface(QString)) );
}

logDialog::~logDialog()
{
    emit changeInterface("");
}

void logDialog::receiveData(frame fr,QString port)
{
    if ( cb_sockets->currentText() != port ) return;
    printRecord(receive,fr);
}

void logDialog::sendData(frame fr,QString port)
{
    if ( cb_sockets->currentText() != port ) return;
    printRecord(send,fr);
}

void logDialog::printRecord(int c, frame fr)
{
    if ( cb_type->currentIndex() != all ) {
        if ( cb_type->currentIndex() == arp && fr.type() != frame::arp ) return;
        if ( cb_type->currentIndex() == ip && fr.type() != frame::ip ) return;
    }
    QString s;
    QTreeWidgetItem *main = new QTreeWidgetItem(lw_log);
    if ( cb_time->isChecked() ) s.append( QTime::currentTime().toString("hh:mm:ss.z")+ " " );
    if ( c == send ) s.append(tr("sent ")); else s.append(tr("received "));
    if ( fr.type() == frame::ip ) s.append(parseIp(fr,main)); else s.append(parseArp(fr,main));
    main->setText(0,s);
    lw_log->scrollToBottom();
}

QString logDialog::parseIp(frame fr,QTreeWidgetItem *parent)
{
    QString s;
    ipPacket p(fr.unpack());
    s.append( p.sender().toString() + " >> " + p.receiver().toString() + tr(" Type: "));
    QTreeWidgetItem *item = new QTreeWidgetItem(parent);
    item->setText(0,fr.toString());
    item->setBackgroundColor(0,cl_frame);
    item = new QTreeWidgetItem(parent);
    item->setText(0,p.toString());
    item->setBackgroundColor(0,cl_ipInternal);
    if ( p.upProtocol() == ipPacket::udp ) {
        udpPacket udp(p.unpack());
        s.append( udp.typeToString());
        QTreeWidgetItem *t = new QTreeWidgetItem(parent);
        t->setText(0,udp.toString());
        t->setBackgroundColor(0,cl_udpInternal);
        switch ( udp.receiver() ) {
            case udpPacket::User : parent->setBackgroundColor(0,cl_user); break;
            case udpPacket::RIP: parent->setBackgroundColor(0,cl_rip); break;
            default: parent->setBackgroundColor(0,cl_undef);
        }
    }
    return s;
}

QString logDialog::parseArp(frame fr,QTreeWidgetItem *parent)
{
    QString s;
    QString type;
    parent->setBackgroundColor(0, cl_arp );
    arpPacket p(fr.unpack());
    if ( p.type() == arpPacket::request ) type = tr("request");
    else type = tr("response");
    if ( p.type() == arpPacket::request) {
        s.append( p.senderIp().toString());
        s.append(tr(" search ") + p.receiverIp().toString() );
    }
    else {
        s.append( p.receiverIp().toString());
        s.append(tr(" found ") + p.senderIp().toString());
    }
    s.append(tr(" Type: ")+"ARP "+type);
    QTreeWidgetItem *item = new QTreeWidgetItem(parent);
    item->setText(0,fr.toString());
    item->setBackgroundColor(0,cl_frame);
    item = new QTreeWidgetItem(parent);
    item->setText(0,tr("ARP-%1:").arg(type));
    item->setBackgroundColor(0, cl_arpInternal);
    QTreeWidgetItem *t = new QTreeWidgetItem(item);
    t->setText(0,tr("sender IP address: %1").arg( p.senderIp().toString() ));
    t->setBackgroundColor(0, cl_arpInternal);
    t = new QTreeWidgetItem(item);
    t->setText(0,tr("sender MAC address: %1").arg( p.senderMac().toString() ));
    t->setBackgroundColor(0, cl_arpInternal);
    t = new QTreeWidgetItem(item);
    t->setText(0,tr("target IP address: %1").arg( p.receiverIp().toString() ));
    t->setBackgroundColor(0, cl_arpInternal);
    t = new QTreeWidgetItem(item);
    t->setText(0,tr("target MAC address: %1").arg( p.receiverMac().toString() ));
    t->setBackgroundColor(0, cl_arpInternal);
    return s;
}

void logDialog::focusOutEvent(QFocusEvent*)
{
    emit changeInterface("");
}

void logDialog::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        retranslateUi(this);
        break;
    default:
        break;
    }
}
