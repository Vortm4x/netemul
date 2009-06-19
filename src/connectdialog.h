#ifndef CONNECTDIALOG_H
#define CONNECTDIALOG_H

#include <QDialog>
class QListWidget;
class QPushButton;
class device;
class devicePort;

class connectDialog : public QDialog
{
    Q_OBJECT
public:
    connectDialog(device *start,device *end);
public slots:
    void changeSelect();
    QString getStart();
    QString getEnd();
private:
    QPushButton *okButton;
    QPushButton *cancelButton;
    QListWidget *startList;
    QListWidget *endList;
    QList<devicePort*> startInterfaceList;
    QList<devicePort*> endInterfaceList;
};

#endif // CONNECTDIALOG_H
