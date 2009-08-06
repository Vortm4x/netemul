#ifndef PROGRAMMREP_H
#define PROGRAMMREP_H

#include <QDataStream>

class device;
class ipPacket;
class smartDevice;

class programmRep
{
public:
    friend class programm;
    programmRep();
    virtual ~programmRep();
    void setSocket(quint16 i) { mySocket = i; }
    quint16 socket() const { return mySocket; }
    void setEnable(bool b) { myEnable = b; }
    bool isEnable() const  { return myEnable; }
    QString name() const { return myName; }
    virtual bool interrupt(int u) = 0;
    /*! Обрабатывает входящее сообщение, должна быть переопределена во
      всех потомках этого класса.
      @param b - ссылка на данные пришедшие программе. */
    virtual void setDevice(smartDevice*) { }
    virtual void execute(ipPacket &p) = 0 ;
    virtual void incTime() { }
    virtual void write(QDataStream &stream) const;
    virtual void read(QDataStream &stream);
protected:
    quint8 countRef;
    bool myEnable; //!< Запущена программа или нет.
    quint16 mySocket; //!< Номер порта на котором программа ждет сообщения.
    QString myName; //!< Имя программы.
};

#endif // PROGRAMMREP_H
