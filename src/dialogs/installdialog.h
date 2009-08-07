#ifndef INSTALLDIALOG_H
#define INSTALLDIALOG_H

#include "ui_installdialog.h"

class smartDevice;

/*!
  Реализует диалог установки программ.
*/
class installDialog : public QDialog, private Ui::installDialog {
    Q_OBJECT
public:
    installDialog(QWidget *parent = 0);
    void setDevice(smartDevice *d);
protected:
    void updateList();
    void changeEvent(QEvent *e);
private:
    smartDevice *smart; //!< Указатель на устройство
public slots:
    void install();
};

#endif // INSTALLDIALOG_H
