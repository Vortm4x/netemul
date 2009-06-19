#ifndef SENDDIALOG_H
#define SENDDIALOG_H

#include <QDialog>
#include <QMap>
#include "macaddress.h"
#include "ipaddress.h"
class QListWidget;
class QListWidgetItem;
class QPushButton;
class QLabel;
class QSlider;
class QSpinBox;
class QCheckBox;
class QRadioButton;
class QVBoxLayout;
class device;
class devicePort;
class interface;

class sendDialog : public QDialog
{
    Q_OBJECT
public:
    enum aim { sender = 0 , receiver = 1 };
    enum { UDP = 25 ,TCP = 26 };
    sendDialog(aim cur,device *t);
    void prepare();
    ipAddress ip() const { return myIp; }
    device* senderDevice() const { return mySenderDevice; }
    bool broadcast() const { return myBroadcast; }
    int messageSize() const { return mySize; }
    int protocol() const { return myProtocol; }
public slots:
    void checkSelected(int cur);
    void checkAccept();
private:
    QMap<QListWidgetItem*,devicePort*> spisok;
    aim myState;
    device *mySenderDevice;
    ipAddress myIp;
    int mySize;
    int myProtocol;
    bool myBroadcast;
    QSpinBox *sizeBox;
    QPushButton *okButton;
    QPushButton *cancelButton;
    QRadioButton *rtn_udp;
    QRadioButton *rtn_tcp;
    QSlider *sizeSlider;
    QLabel *caption;
    QLabel *sizeCaption;
    QListWidget *list;
    QCheckBox *check;
    QVBoxLayout *all;
};

#endif // SENDDIALOG_H
