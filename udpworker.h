#ifndef UDPWORKER_H
#define UDPWORKER_H

#include <QUdpSocket>
#include <QNetworkDatagram>
#include <QDateTime>


#define BIND_PORT 12345
#define USER_BIND_PORT 12346

class UDPworker : public QObject
{
    Q_OBJECT
public:
    explicit UDPworker(QObject *parent = nullptr);
    void InitSocket( void );
    void ReadDatagram( QNetworkDatagram datagram);
    void SendDatagram(QByteArray data );

    void user_SendDatagram(QByteArray data);
    void ReadUserDatagram(QNetworkDatagram datagram);



private slots:
    void readPendingDatagrams(void);
    void readPendingUserDatagrams(void);

private:
    QUdpSocket* serviceUdpSocket;
    QUdpSocket* userUdpSocket;

signals:
    void sig_sendTimeToGUI(QDateTime data);
    void sig_sendInfoToGUI(QHostAddress senderAddress, QString size);

};

#endif // UDPWORKER_H
