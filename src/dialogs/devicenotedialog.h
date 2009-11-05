#ifndef DEVICENOTEDIALOG_H
#define DEVICENOTEDIALOG_H

#include "ui_devicenotedialog.h"

class deviceSetting;

class deviceNoteDialog : public QDialog, private Ui::deviceNoteDialog {
    Q_OBJECT
public:
    deviceNoteDialog(QWidget *parent = 0);
    void setDevice(deviceSetting *device);
    ~deviceNoteDialog();
public slots:
    void apply();
protected:
    void changeEvent(QEvent *e);
private:
    deviceSetting *myDevice;
};

#endif // DEVICENOTEDIALOG_H
