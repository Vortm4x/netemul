#ifndef SCENECONTROL_H
#define SCENECONTROL_H

#include <QtCore/QObject>
#include <QtGui/QIcon>

class myCanvas;
class logDialog;

class sceneControl : public QObject
{
    Q_OBJECT
public:
    sceneControl(QObject *parent,myCanvas *s);
    bool isSelect() const;
    QString tableName() const;
    QString note() const;
    QStringList sockets() const;
    QIcon tableIcon() const;
    QString deviceName() const;
private:
    myCanvas *scene;
public slots:
    void observeSelection();
    void propertyDialog() const;
    void tableDialog() const;
    void adapterDialog() const;
    void programmsDialog() const;
    void arpDialog() const;
    void showLogDialog(logDialog *log) const;
signals:
    void selectOneDevice(bool);
    void selectTableDevice(bool);
    void selectSmartDevice(bool);
};

#endif // SCENECONTROL_H
