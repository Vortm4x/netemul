#ifndef INTERFACE_H
#define INTERFACE_H

#include <QObject>

class Interface;
typedef QList<Interface*> InterfaceList;

class Interface : public QObject
{
    Q_OBJECT
    Q_PROPERTY( QString ip READ ip WRITE setIp )
public:
    explicit Interface(QObject *parent = 0);

    void setIp(const QString &ad) { m_ip = ad; }
    QString ip() const { return m_ip; }

signals:

public slots:

private:
    QString m_ip;
};

#endif // INTERFACE_H
