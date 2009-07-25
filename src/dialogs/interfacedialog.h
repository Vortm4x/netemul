#ifndef INTERFACEDIALOG_H
#define INTERFACEDIALOG_H

#include <QDialog>

class interface;


class QListWidget;
class QPushButton;
class QLabel;


class interfaceDialog : public QDialog
{
    Q_OBJECT
public:
    interfaceDialog();
    int  result() {  return res;  }
public slots:
    void checkAccept();
    void changeItem(int n);
private:
    int res;

    QLabel *picture;
    QLabel *speed;

    QListWidget *standarts;
    QPushButton *addEditButton;
    QPushButton *cancelButton;
};

#endif // INTERFACEDIALOG_H
