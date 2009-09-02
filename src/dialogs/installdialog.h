#ifndef INSTALLDIALOG_H
#define INSTALLDIALOG_H

#include "ui_installdialog.h"

class smartDevice;

static const int MAGIC_START_PROGRAMM_NUMBER = 50;

/*!
  Реализует диалог установки программ.
*/
class installDialog : public QDialog, private Ui::installDialog {
    Q_OBJECT
public:
    installDialog(QWidget *parent = 0);
    void setDevice(smartDevice *d);
protected:
    void changeEvent(QEvent *e);
private:
    smartDevice *smart; //!< Указатель на устройство
public slots:
    void install();
};

#endif // INSTALLDIALOG_H
