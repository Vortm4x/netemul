#ifndef DHCPSERVERPROPERTY_H
#define DHCPSERVERPROPERTY_H

#include <ui_dhcpserverproperty.h>
#include "ipedit.h"

class dhcpServerProgramm;

class dhcpServerProperty : public QDialog, private Ui::dhspServerProperty
{
    Q_OBJECT
public:
    dhcpServerProperty(QWidget *parent = 0);
    void setProgramm( dhcpServerProgramm *prog );
public slots:
    void addRecord();
    void deleteRecord();
    void changeState(bool);
    void apply();
protected:
    void changeEvent(QEvent *e);
private:
    dhcpServerProgramm *myProgramm;
    ipEdit *begin;
    ipEdit *end;
    ipEdit *mask;
    ipEdit *gatew;
};

#endif // DHCPSERVERPROPERTY_H
