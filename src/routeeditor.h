#ifndef ROUTEEDITOR_H
#define ROUTEEDITOR_H

#include <QDialog>
class QTableWidget;
class QPushbutton;
class QSpinBox;
class QComboBox;

class ipEdit;
class smartDevice;
/*!
  Реализует отображение таблицы маршрутизации.
*/
class routeEditor : public QDialog
{
    Q_OBJECT
public:
    routeEditor();
    ~routeEditor();
    void updateTable();
    void correctSize();
    void setDevice(smartDevice *s);
private:
    void readSetting();
    void writeSetting() const;
    ipEdit *ip_dest;
    ipEdit *ip_mask;
    ipEdit *ip_gateway;
    QComboBox *cb_out;
    QSpinBox *sp_metr;
    QPushButton *btn_add;
    QPushButton *btn_remove;
    QPushButton *btn_close;
    QTableWidget *table;
    smartDevice *dev;
protected:
    void resizeEvent(QResizeEvent *e);
public slots:
    void addRecord();
    void deleteRecord();
    void checkSelection();
};
//--------------------------------------------------

#endif // ROUTEEDITOR_H
