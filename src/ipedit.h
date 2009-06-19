#ifndef IPEDIT_H
#define IPEDIT_H

#include <QWidget>
#include "ipaddress.h"

class QLineEdit;
class QLabel;

class ipEdit : public QWidget
{
Q_OBJECT
public:
    ipEdit(QString str,QWidget *parent = 0);
    void setText(QString str);
    ipAddress ipText() const;
    QString text() const;
    void clear();
private:
    QLabel *label;
    QLineEdit *part[4];
signals:
    void textChanged(QString);
};

#endif // IPEDIT_H
