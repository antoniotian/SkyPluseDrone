#include "udp.h"
#include <QNetworkDatagram>

UDP::UDP(QObject *parent)
    : QObject{parent}, udpSocket(new QUdpSocket(this))
{
    udpThread = new QThread(this);
    connect(udpThread, &QThread::started, this, &UDP::udpInitial);
    connect(udpThread, &QThread::finished, this, &QObject::deleteLater);
    this->moveToThread(udpThread);
    udpThread->start();
}

UDP::~UDP()
{
    if (udpThread->isRunning()){
        udpThread->wait();
        udpThread->quit();
    }
}


void UDP::udpInitial()
{
    connect(udpSocket, &QUdpSocket::readyRead, this, &UDP::readPendingDatagrams);
    connect(udpSocket, &QUdpSocket::connected, this, &UDP::onConnected);
    connect(udpSocket, &QUdpSocket::disconnected, this, &UDP::onDisconnected);
    connect(udpSocket, &QUdpSocket::errorOccurred, this, &UDP::onErrorOccurred);
}


void UDP::startUDPServer(quint16 port)
{
    udpSocket->bind(port, QUdpSocket::ShareAddress);
}


void UDP::stopUDPServer()
{
    udpSocket->close();
}


void UDP::readPendingDatagrams()
{
    qDebug() << "UDP Current Thread ID: " << QThread::currentThreadId();
    while (udpSocket->hasPendingDatagrams()) {
        QNetworkDatagram datagram = udpSocket->receiveDatagram();
        // QString message = QString::fromUtf8(datagram.data());
        // emit messageReceived(message);
    }
}


/*UDP连接成功信号*/
void UDP::onConnected()
{
    emit sig_serverStarted();
}


/*UDP断开成功信号*/
void UDP::onDisconnected()
{
    emit sig_serverStopped();
}


/*UDP错误信号*/
void UDP::onErrorOccurred()
{
    emit sig_udpError();
}
