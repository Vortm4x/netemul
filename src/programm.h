#ifndef PROGRAMM_H
#define PROGRAMM_H

#include <QObject>
class device;

/*!
  Абстрактный класс от которого будут унаследованы все программы.
  Программа устанавливаеться на устройство и ждет сообщение на определенном порту,
  как и реальная программа она обрабатывает входящие udp и tcp сообщения на своем порту.
*/
class programm : public QObject
{
    Q_OBJECT
public:
    programm(device *d);
    /*! Задает номер порта
        @param i - номер порта */
    void setSocket(quint16 i) { mySocket = i; }
    /*! @return Номер порта */
    quint16 socket() const { return mySocket; }
    /*! Обрабатывает входящее сообщение, должна быть переопределена во
      всех потомках этого класса.
      @param b - ссылка на данные пришедшие программе. */
    virtual void execute(QByteArray &b) = 0 ;
protected:
    quint16 mySocket; //!< Номер порта на котором программа ждет сообщения.
};

#endif // PROGRAMM_H
