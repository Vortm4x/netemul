#ifndef TABLEARP_H
#define TABLEARP_H

#include <QDialog>

class QTableWidget;
class QPushButton;
class QLineEdit;
class QComboBox;
class QStringList;
class smartDevice;
class ipEdit;
class macAddress;
class arpModel;

class tableArp : public QDialog
{
    Q_OBJECT
    Q_DISABLE_COPY(tableArp)
public:
    tableArp();
    void setDevice(smartDevice *dev);
    void updateTable();
    void addItem(int r, int c, QString s);
private:
    smartDevice *device;
    QTableWidget *table;
    QPushButton *btn_add;
    QPushButton *btn_del;
    QPushButton *btn_close;
    QLineEdit *le_mac;
    QComboBox *cb_port;
    QStringList s;
    QList<arpModel*> list;
    ipEdit *ip;
public slots:
    void addRecord();
    void deleteRecord();
    void checkSelection();
};

#endif // TABLEARP_H

