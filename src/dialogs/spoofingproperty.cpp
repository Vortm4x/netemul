#include "spoofingproperty.h"
#include "spoofingprogram.h"
#include "ui_spoofingproperty.h"

spoofingProperty::spoofingProperty(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::spoofingProperty)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    connect( ui->okButton , SIGNAL(clicked()) , this , SLOT(apply()) );
}

spoofingProperty::~spoofingProperty()
{
    delete ui;
}

void spoofingProperty::setProgramm(SpoofingProgram *p)
{
    ui->clientEdit->setText( p->clientIp().toString() );
    ui->serverEdit->setText( p->serverIp().toString() );
    pr = p;
}

void spoofingProperty::apply()
{
    pr->setClientIp( ui->clientEdit->ipAddress() );
    pr->setServerIp( ui->serverEdit->ipAddress() );
    accept();
}

void spoofingProperty::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
