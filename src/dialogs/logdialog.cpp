#include "logdialog.h"
#include "frame.h"
#include "ippacket.h"
#include "arppacket.h"
#include <QtDebug>

logDialog::logDialog()
{
    setupUi(this);
    startTimer(100);
    count = 0;
    temp = "";
}

logDialog::~logDialog()
{

}

void logDialog::receiveData(frame fr)
{
    printRecord(receive,fr);
}

void logDialog::sendData(frame fr)
{
    printRecord(send,fr);
}

void logDialog::routerChange(bool isRouter)
{
    if ( isRouter ) te_log->append(tr("Routing has been <b>enabled</b>"));
    else te_log->append(tr("Routing has been <b>disabled</b>"));
}

void logDialog::printRecord(int c, frame fr)
{
    QString s;
    if ( cb_type->currentIndex() == 0 ) {
        if ( c == send ) s.append("send "); else s.append("receive ");
        if ( fr.type() == frame::ip ) s.append(parseIp(fr)); else s.append(parseArp(fr));
    }
    if ( cb_type->currentIndex() == 1 ) {
        if ( fr.type() != frame::arp ) return;
        if ( c == send ) s.append("send "); else s.append("receive ");
        s.append(parseArp(fr));
    }
    if ( cb_type->currentIndex() == 2 ) {
        if ( fr.type() != frame::ip ) return;
        if ( c == send ) s.append("send "); else s.append("receive ");
        s.append(parseIp(fr));
    }
    if ( temp.isEmpty() ) temp = s;
    else {
        if ( temp == s ) count++;
        else {
            if ( count ) temp += tr("Count: %1").arg(count+1);
            te_log->append(temp);
            temp = "";
            count = 0;
            te_log->append(s);
        }
    }
}

QString logDialog::parseIp(frame fr)
{
    QString s;
    ipPacket p(fr.unpack());
    s.append("ip-packet: <b>");
    s.append(p.sender().toString() + "</b> ");
    if ( chb_mac->isChecked() ) s.append("( " + fr.sender().toString() + ") ");
    s.append(">>  <b>" + p.receiver().toString() + "</b> ");
    if ( chb_mac->isChecked() ) s.append("( " + fr.receiver().toString() + ") ");
    return s;
}

QString logDialog::parseArp(frame fr)
{
    QString s;
    arpPacket p(fr.unpack());
    if ( p.type() == arpPacket::request) {
        s.append("arp-request: <b>" + p.senderIp().toString() + "</b> ");
        if ( chb_mac->isChecked() ) s.append("( " + fr.sender().toString() + ") ");
        s.append("search <b>" + p.receiverIp().toString() + "</b> " );
        if ( chb_mac->isChecked() ) s.append("( " + fr.receiver().toString() + ") ");
    }
    else { s.append("arp-response: <b>" + p.receiverIp().toString() + "</b> ");
        if ( chb_mac->isChecked() ) s.append("( " + fr.sender().toString() + ") ");
        s.append("found <b>" + p.senderIp().toString() + "</b> ");
        if ( chb_mac->isChecked() ) s.append("( " + fr.receiver().toString() + ") ");
    }
    return s;
}

void logDialog::timerEvent(QTimerEvent*)
{
    if ( temp.isEmpty() ) return;
    if ( count ) temp += tr("Count: %1").arg(count+1);
    te_log->append(temp);
    temp = "";
    count = 0;
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
