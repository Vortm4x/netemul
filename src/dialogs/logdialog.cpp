#include "logdialog.h"
#include "frame.h"
#include "ippacket.h"
#include "arppacket.h"
#include <QtDebug>

logDialog::logDialog()
{
    setupUi(this);
}

logDialog::~logDialog()
{

}

void logDialog::receiveData(frame fr)
{
    Q_UNUSED(fr);
}

void logDialog::sendData(frame fr)
{
    QString receiver;
    QString sender;
    QString s;
    if ( cb_type->currentIndex() == 0 ) {
        if ( fr.type() == frame::ip ) {
            ipPacket p(fr.unpack());
            s.append("send: ip-packet to ");
            receiver = p.receiver().toString();
            sender = p.sender().toString();
        } else {
            arpPacket p(fr.unpack());
            if ( p.type() == arpPacket::request) s.append("send: arp-request to ");
            else s.append("send: arp-response to ");
            receiver = p.receiverIp().toString();
            sender = p.senderIp().toString();
        }        
    }
    if ( cb_type->currentIndex() == 1 )
        if ( fr.type() == frame::arp ) {
            arpPacket p(fr.unpack());
            if ( p.type() == arpPacket::request) s.append("receive: arp-request to ");
            else s.append("receive: arp-response to ");
            receiver = p.receiverIp().toString();
            sender = p.senderIp().toString();
        }
    if ( cb_type->currentIndex() == 2 )
        if ( fr.type() == frame::ip ) {
            ipPacket p(fr.unpack());
            s.append("receive: ip-packet to ");
            receiver = p.receiver().toString();
            sender = p.sender().toString();
        }
    s.append(receiver + " (" + fr.receiver().toString() + ") by  " + sender + " (" + fr.sender().toString() + ") ");
    te_log->append(s);
}

void logDialog::routerChange(bool isRouter)
{
    if ( isRouter ) te_log->append("Routing has been <b>enabled</b>");
    else te_log->append("Routing has been <b>disabled</b>");
}

void logDialog::printRecord(int c, frame fr)
{
//    QString receiver;
//    QString sender;
//    QString s;
//    if ( cb_type->currentIndex() == 0 ) {
//        if ( fr.type() == frame::ip ) {
//            te_log->append(parseIp(c,fr));
//        } else {
//
//            receiver = p.receiverIp().toString();
//            sender = p.senderIp().toString();
//        }
//    }
//    if ( cb_type->currentIndex() == 1 )
//        if ( fr.type() == frame::arp ) {
//            arpPacket p(fr.unpack());
//            if ( p.type() == arpPacket::request) s.append("receive: arp-request to ");
//            else s.append("receive: arp-response to ");
//            receiver = p.receiverIp().toString();
//            sender = p.senderIp().toString();
//        }
//    if ( cb_type->currentIndex() == 2 )
//        if ( fr.type() == frame::ip ) {
//            ipPacket p(fr.unpack());
//            s.append("receive: ip-packet to ");
//            receiver = p.receiver().toString();
//            sender = p.sender().toString();
//        }
//    s.append(receiver + " (" + fr.receiver().toString() + ") by  " + sender + " (" + fr.sender().toString() + ") ");
//}
//
//QString logDialog::parseIp(int c, frame fr)
//{
//    QString s;
//    ipPacket p(fr.unpack());
//    if ( c == send ) s.append("send: ip-packet to ");
//    else s.append("receive: ip-packet by ");
//    s.append(p.receiver().toString() + " (" + fr.receiver().toString() + ") ");
//    if ( c == send) s.append("by ");
//    else s.append("to ");
//    s.append(p.sender().toString() + " (" + fr.sender().toString() + ") ");
//    return s;
}

QString logDialog::parseArp(int c,frame fr)
{
    QString s;
    arpPacket p(fr.unpack());
    if (c == send) s.append("send: ");
    else s.append("receive: ");
    if ( p.type() == arpPacket::request) s.append("arp-request to ");
    else s.append("arp-response to ");
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
