#ifndef PROGRAMM_H
#define PROGRAMM_H

#include <QObject>
class device;
class ipPacket;

/*!
  Абстрактный класс от которого будут унаследованы все программы.
  Программа устанавливаеться на устройство и ждет сообщение на определенном порту,
  как и реальная программа она обрабатывает входящие udp и tcp сообщения на своем порту.
*/
class programm : public QObject
{
    Q_OBJECT
public:
    programm();
    void setSocket(quint16 i) { mySocket = i; }
    quint16 socket() const { return mySocket; }
    void setEnable(bool b) { myEnable = b; }
    bool isEnable() const  { return myEnable; }
    QString name() const { return myName; }
    /*! Обрабатывает входящее сообщение, должна быть переопределена во
      всех потомках этого класса.
      @param b - ссылка на данные пришедшие программе. */
    virtual void execute(ipPacket *p) = 0 ;
    virtual void incTime() { }
protected:
    bool myEnable; //!< Запущена программа или нет.
    quint16 mySocket; //!< Номер порта на котором программа ждет сообщения.
    QString myName; //!< Имя программы.
};

#endif // PROGRAMM_H
