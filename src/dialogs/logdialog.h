#ifndef LOGDIALOG_H
#define LOGDIALOG_H

#include "ui_logdialog.h"
#include "frame.h"

static const QColor cl_frame = "#fbec5d";
static const QColor cl_ipInternal = "#efaf8c";
static const QColor cl_udpInternal = "#ccccff";
static const QColor cl_rip = "#f78d9d";
static const QColor cl_user = "#badbad";
static const QColor cl_undef = "#f984e5";
static const QColor cl_arp = "#7fc7ff";
static const QColor cl_arpInternal = "#c0c0c0";
static const QColor cl_tcpInternal = "#008080";
static const QColor cl_tcpData = "#c0c0c0";

class logDialog : public QWidget , private Ui::logDialog
{
    Q_OBJECT
    Q_DISABLE_COPY(logDialog)
public:
    logDialog(QStringList list);
    ~logDialog();
    enum { send = 0, receive = 1 };
    enum { all = 0 , arp = 1 , tcp = 2 , udp = 3 };
signals:
    void changeInterface(QString);
public  slots:
    void receiveData(frame fr,QString port);
    void sendData(frame fr,QString port);
protected:
    bool eventFilter(QObject *obj,QEvent *e);
    void changeEvent(QEvent *e);
private:
    void printRecord( int c, frame fr );
    QString parseIp(frame fr,QTreeWidgetItem *parent);
    QString parseArp(frame fr,QTreeWidgetItem *parent);
};

#endif // LOGDIALOG_H
