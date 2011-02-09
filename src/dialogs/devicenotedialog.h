#ifndef DEVICENOTEDIALOG_H
#define DEVICENOTEDIALOG_H

#include "ui_devicenotedialog.h"

class DeviceSetting;

class deviceNoteDialog : public QDialog, private Ui::deviceNoteDialog {
    Q_OBJECT
public:
    deviceNoteDialog(QWidget *parent = 0);
    void setDevice(DeviceSetting *device);
    ~deviceNoteDialog();
public slots:
    void apply();
protected:
    void changeEvent(QEvent *e);
private:
    DeviceSetting *myDevice;
};

#endif // DEVICENOTEDIALOG_H
