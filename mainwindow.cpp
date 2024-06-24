#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    udpWorker = new UDPworker(this);
    udpWorker->InitSocket();

    connect(udpWorker, &UDPworker::sig_sendTimeToGUI, this, &MainWindow::DisplayTime);

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [&]{

        QDateTime dateTime = QDateTime::currentDateTime();

        QByteArray dataToSend;
        QDataStream outStr(&dataToSend, QIODevice::WriteOnly);

        outStr << dateTime;

        udpWorker->SendDatagram(dataToSend);
        timer->start(TIMER_DELAY);

    });

    connect(ui->pb_sendDatagram, &QPushButton::clicked, this, [&]{
        QByteArray userDataToSend;
        QDataStream user_outStr(&userDataToSend, QIODevice::WriteOnly);
        user_outStr << ui->le_textDatagram->text();

        udpWorker->user_SendDatagram(userDataToSend);
    });

    connect(udpWorker, UDPworker::sig_sendInfoToGUI, this, &MainWindow::DisplayInfo);

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pb_start_clicked()
{
    timer->start(TIMER_DELAY);
}


void MainWindow::DisplayTime(QDateTime data)
{
    counterPck++;
    if(counterPck % 20 == 0){
        ui->te_result->clear();
    }

    ui->te_result->append("Текущее время: " + data.toString() + ". "
                "Принято пакетов " + QString::number(counterPck));


}

void MainWindow::DisplayInfo(QHostAddress senderAddress, QString size){

    QString sender = senderAddress.toString();

    ui->te_result->append("Принято сообщение от " + sender +
                          " размер сообщения(байт)" + size);
}

void MainWindow::on_pb_stop_clicked()
{

    timer->stop();
}

