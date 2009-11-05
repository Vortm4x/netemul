#include "devicenotedialog.h"
#include "deviceimpl.h"

deviceNoteDialog::deviceNoteDialog(QWidget *parent) : QDialog(parent)
{
    setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
}

void deviceNoteDialog::setDevice(deviceSetting *device)
{
    myDevice = device;
    te_text->setPlainText( device->note() );
}

deviceNoteDialog::~deviceNoteDialog()
{
    delete myDevice;
}

void deviceNoteDialog::apply()
{
    myDevice->setNote( te_text->toPlainText() );
    accept();
}

void deviceNoteDialog::changeEvent(QEvent *e)
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
