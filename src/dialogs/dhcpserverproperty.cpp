#include "dhcpserverproperty.h"
#include "dhcpserverprogramm.h"

dhcpServerProperty::dhcpServerProperty(QWidget *parent) : QDialog(parent)
{
    setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    begin = new ipEdit(tr("From "));
    begin->setEnabled(false);
    end = new ipEdit(tr("to"));
    end->setEnabled(false);
    mask = new ipEdit(tr("Mask"));
    mask->setEnabled(false);
    gatew = new ipEdit(tr("Gateway"));
    gatew->setEnabled(false);
    QHBoxLayout *l = new QHBoxLayout;
    l->addWidget(begin);
    l->addWidget(end);
    lay->addLayout(l);
    l = new QHBoxLayout;
    l->addWidget(mask);
    l->addWidget(gatew);
    lay->addLayout(l);
    QStringList list;
    list << tr("Mac") << tr("Ip") << tr("Mask") << tr("Gateway");
    tw_static->setHorizontalHeaderLabels(list);    
}

void dhcpServerProperty::setProgramm(dhcpServerProgramm *prog)
{
    myProgramm = prog;
}

void dhcpServerProperty::addRecord()
{
    tw_static->insertRow(tw_static->rowCount());
}

void dhcpServerProperty::deleteRecord()
{
    tw_static->removeRow(tw_static->currentRow());
}

void dhcpServerProperty::changeState(bool b)
{
    begin->setEnabled(b);
    end->setEnabled(b);
    mask->setEnabled(b);
    gatew->setEnabled(b);
}

void dhcpServerProperty::apply()
{
    for ( int i = 0; i < tw_static->rowCount(); i++ ) {
        staticRecord *rec = new staticRecord;
        rec->chaddr = macAddress(tw_static->item(i,0)->text());
        rec->yiaddr = ipAddress(tw_static->item(i,1)->text());
        rec->mask = ipAddress(tw_static->item(i,2)->text());
        rec->gateway = ipAddress(tw_static->item(i,3)->text());
        rec->time = sb_time->value();
        if ( !myProgramm->containRecord(rec) ) myProgramm->addStaticRecord(rec);
    }
    accept();
}

void dhcpServerProperty::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        retranslateUi(this);
        break;
    default:
        break;
    }
}
