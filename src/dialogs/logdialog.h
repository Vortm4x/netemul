#ifndef LOGDIALOG_H
#define LOGDIALOG_H

#include "ui_logdialog.h"
#include "frame.h"

class logDialog : public QWidget , private Ui::logDialog
{
    Q_OBJECT
    Q_DISABLE_COPY(logDialog)
public:
    logDialog();    
    ~logDialog();
    enum { send = 0, receive = 1 };
public  slots:
    void receiveData(frame fr);
    void sendData(frame fr);
    void routerChange(bool isRouter);    
protected:
    void changeEvent(QEvent *e);
    void printRecord( int c, frame fr );
    QString parseIp(int c, frame fr);
    QString parseArp(int c, frame fr);
};

#endif // LOGDIALOG_H
