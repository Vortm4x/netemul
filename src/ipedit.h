#ifndef IPEDIT_H
#define IPEDIT_H

#include <QWidget>
#include "ipaddress.h"

class QLineEdit;
class QLabel;
/**
  Виджет для ввода ip адреса.
*/
class ipEdit : public QWidget
{
Q_OBJECT
public:
    ipEdit(QString str,QWidget *parent = 0);
    void setText(QString str);
    ipAddress ipText() const { return ipAddress(text()); }
    QString text() const;
    void clear();   
public slots:
    void setDefaultMask(quint8 u);
    void changeMask(QString s);
private:
    QLabel *label;
    QLineEdit *part[4];
signals:
    void textChanged(QString);
    void maskChanged(quint8);
};
//-----------------------------------------
#endif // IPEDIT_H
