#ifndef PROGRAMMDIALOG_H
#define PROGRAMMDIALOG_H

#include "ui_programmdialog.h"

class smartDevice;

/*!
  Реализует диалог установленных пограмм устройства.
*/
class programmDialog : public QDialog, private Ui::programmDialog {
    Q_OBJECT
    Q_DISABLE_COPY(programmDialog)
public:
    programmDialog(QWidget *parent = 0);
    void setDevice( smartDevice *d );
protected:
    void updateList();
    void changeEvent(QEvent *e);
private:
    smartDevice *s; //!< Указатель на устройство.
public slots:
    void apply();
    void add();
    void remove();
};

#endif // PROGRAMMDIALOG_H
