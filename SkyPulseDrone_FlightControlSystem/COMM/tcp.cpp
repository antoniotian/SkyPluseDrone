#include "tcp.h"

TCP::TCP(QObject *parent)
    : QTcpServer(parent)
{
    TCPThread = new QThread(this);
    connect(TCPThread, &QThread::started, this, &TCP::startServer);
    connect(TCPThread, &QThread::finished, TCPThread, &QObject::deleteLater);
    this->moveToThread(TCPThread);
    TCPThread->start();
}


TCP::~TCP()
{
    if (TCPThread->isRunning()){
        TCPThread->wait();
        TCPThread->quit();
    }
}


void TCP::startServer()
{
    qDebug() << "TCP Initialized Thread:" << QThread::currentThreadId();
    if(this->listen(QHostAddress::Any, PORT)){
        qDebug() << "TCP Server started on port:" << PORT;
    }
    else{
        qDebug() << "TCP Server failed to start:" << this->errorString();
    }
}


void TCP::stopServer()
{
    QTcpSocket *client = qobject_cast<QTcpSocket*>(sender());
    if(client){
        clients.removeAll(client);
        client->deleteLater();
        qWarning("Client disconnected");
    }
}


void TCP::onReadyRead()
{
    // Any function here
    QTcpSocket *clients = qobject_cast<QTcpSocket*>(sender());
    if(!clients) return;
    QByteArray data = clients->readAll();
//    this->dataTranslator(data);
}


void TCP::incomingConnection(qintptr socketDescriptor)
{
    QTcpSocket *client = new QTcpSocket(this);
    client->setSocketDescriptor(socketDescriptor);

    connect(client, &QTcpSocket::disconnected, this, &TCP::stopServer);
    connect(client, &QTcpSocket::readyRead, this, &TCP::onReadyRead);

    clients.append(client);
    qDebug() << "Client connected:" << client->peerAddress().toString();
}


void TCP::sendMessage64Bytes(const QByteArray &datapackage_u64)
{
    QByteArray messageToSend = datapackage_u64;
    // Make sure the message is exactly 64 bytes
    broadcastMessage(messageToSend);
}


void TCP::broadcastMessage(const QByteArray &message)
{
    // BroadcastMessage
    qDebug() << "The TCP Thread ID is:" << QThread::currentThreadId();
    for(QTcpSocket *client : qAsConst(clients)){
        if(client->state() == QTcpSocket::ConnectedState){
            client->write(message);
        }
    }
}
