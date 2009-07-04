#ifndef TABLEARP_H
#define TABLEARP_H

#include <QDialog>

class QTableWidget;
class QPushButton;
class QLineEdit;
class QComboBox;
class smartDevice;
class ipEdit;
class macAddress;

class tableArp : public QDialog
{
    Q_OBJECT
    Q_DISABLE_COPY(tableArp)
public:
    tableArp(QWidget *parent = 0);
    void setDevice(smartDevice *dev);
    void correctSize();
    void updateTable();
private:
    smartDevice *d;
    QTableWidget *table;
    QPushButton *btn_add;
    QPushButton *btn_del;
    QPushButton *btn_close;
    QLineEdit *le_mac;
    QComboBox *cb_port;
    ipEdit *ip;
protected:
    void resizeEvent(QResizeEvent *e) { Q_UNUSED(e); correctSize(); }
public slots:
    void addRecord();
    void deleteRecord();
};

#endif // TABLEARP_H

