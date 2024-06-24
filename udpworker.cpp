#include "udpworker.h"

UDPworker::UDPworker(QObject *parent) : QObject(parent)
{



}


/*!
 * @brief Метод инициализирует UDP сервер
 */
void UDPworker::InitSocket()
{

    serviceUdpSocket = new QUdpSocket(this);
    userUdpSocket = new QUdpSocket(this);
    /*
     * Соединяем присваиваем адрес и порт серверу и соединяем функцию
     * обраотчик принятых пакетов с сокетом
     */
    serviceUdpSocket->bind(QHostAddress::LocalHost, BIND_PORT);
    userUdpSocket->bind(QHostAddress::LocalHost, USER_BIND_PORT);

    connect(serviceUdpSocket, &QUdpSocket::readyRead, this, &UDPworker::readPendingDatagrams);
    connect(userUdpSocket, &QUdpSocket::readyRead, this, &UDPworker::readPendingUserDatagrams);

}

/*!
 * @brief Метод осуществляет обработку принятой датаграммы
 */
void UDPworker::ReadDatagram(QNetworkDatagram datagram)
{

    QByteArray data;
    data = datagram.data();


    QDataStream inStr(&data, QIODevice::ReadOnly);
    QDateTime dateTime;
    inStr >> dateTime;

    emit sig_sendTimeToGUI(dateTime);
}
/*!
 * @brief Метод осуществляет опередачу датаграммы
 */
void UDPworker::SendDatagram(QByteArray data)
{
    /*
     *  Отправляем данные на localhost и задефайненный порт
     */
    serviceUdpSocket->writeDatagram(data, QHostAddress::LocalHost, BIND_PORT);

}

/*!
 * @brief Метод осуществляет чтение датаграм из сокета
 */
void UDPworker::readPendingDatagrams( void )
{
    /*
     *  Производим чтение принятых датаграмм
     */
    while(serviceUdpSocket->hasPendingDatagrams()){
            QNetworkDatagram datagram = serviceUdpSocket->receiveDatagram();
            ReadDatagram(datagram);
    }

}

//метод отправляет датаграмму
void UDPworker::user_SendDatagram(QByteArray data)
{

    userUdpSocket->writeDatagram(data, QHostAddress::LocalHost, USER_BIND_PORT);
}


//метод принимает
void UDPworker::readPendingUserDatagrams( void ){
    while(userUdpSocket->hasPendingDatagrams()){
            QNetworkDatagram datagram = userUdpSocket->receiveDatagram();
            ReadUserDatagram(datagram);

    }
}
//метод передает датаграмму
void UDPworker::ReadUserDatagram(QNetworkDatagram datagram){

    QHostAddress senderAddress = datagram.senderAddress();
    QString size = QString (" %1").arg(datagram.data().size()-4);//вычитаем размер заголовков

    emit sig_sendInfoToGUI(senderAddress, size);
}
