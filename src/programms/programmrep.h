#ifndef PROGRAMMREP_H
#define PROGRAMMREP_H

#include <QDataStream>

class device;
class smartDevice;

class programmRep : public QObject
{
    Q_OBJECT
public:
    friend class programm;
    programmRep();
    virtual ~programmRep();
    virtual void setEnable(bool b) { myEnable = b; }
    bool isEnable() const  { return myEnable; }
    QString name() const { return myName; }
    virtual void setDevice(smartDevice *s) { sd = s; }
    virtual bool interrupt(int u) = 0;
    virtual void incTime() { }
    virtual void write(QDataStream &stream) const;
    virtual void read(QDataStream &stream);
protected:
    smartDevice *sd;
    QString myName; //!< Имя программы.
private:
    quint8 countRef;
    bool myEnable;
};

#endif // PROGRAMMREP_H
