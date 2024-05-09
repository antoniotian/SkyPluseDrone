#include "tcp.h"

TCP::TCP(QObject *parent)
    : QObject{parent}, TCPSocket(new QTcpSocket(this))
{
    TCPThread = new QThread(this);
    connect(TCPThread, &QThread::started, this, &TCP::tcpInitial);
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

/*TCP服务初始化*/
void TCP::tcpInitial()
{
    connect(TCPSocket, &QTcpSocket::connected, this, &TCP::onConnected);
    connect(TCPSocket, &QTcpSocket::disconnected, this, &TCP::onDisconnected);
    connect(TCPSocket, &QTcpSocket::errorOccurred, this, &TCP::onErrorOccurred);
    connect(TCPSocket, &QTcpSocket::readyRead, this, &TCP::readMessage);
}

/*TCP服务启动*/
void TCP::startTCPServer(const QString &host_ip, quint16 port)
{
    TCPSocket->connectToHost(host_ip, port);
}

/*TCP服务断开连接函数*/
void TCP::stopTCPServer()
{
    if(TCPSocket->state() == QTcpSocket::ConnectedState){  // 判断连接状态之后再断开连接
        TCPSocket->disconnectFromHost();
    }
}

/*TCP数据发送函数*/
void TCP::sendMessage(const QByteArray &message)
{
    if(TCPSocket->state() == QTcpSocket::ConnectedState)
    {
        TCPSocket->write(message);
    }
}

/*TCP数据接收函数*/
void TCP::readMessage()
{
    QByteArray data = TCPSocket->readAll();
    qDebug() << "TCP Thread ID: " << QThread::currentThreadId();
    dataDecoder(data);
}

/*TCP连接成功信号*/
void TCP::onConnected()
{
    emit sig_serverStarted(); // 当连接成功时发出信号
}

/*TCP断开成功信号*/
void TCP::onDisconnected()
{
    emit sig_serverStopped(); // 当断开连接成功时发出信号
}

/*TCP错误信号*/
void TCP::onErrorOccurred()
{
    emit sig_tcpError(); // 当连接错误时发出信号
}

/*TCP数据解码器*/
void TCP::dataDecoder(const QByteArray &data_to_decode)
{
    // Create a runnable lambda function
    auto runnable = [this, data_to_decode](){
        QDataStream stream(data_to_decode);
        stream.setByteOrder(QDataStream::LittleEndian);

        float ax, ay, az, gx, gy, gz, mx, my, mz, headingDeg, pressure, temperature;

        stream >> ax >> ay >> az
            >> gx >> gy >> gz
            >> mx >> my >> mz
            >> headingDeg >> pressure >> temperature ;

        if (stream.status() != QDataStream::Ok) {
            qWarning() << "Failed to decode data from stream";
            return;
        }

        QStringList dataList;
        dataList.reserve(12); // 优化点：预分配QStringList的大小
        dataList << QString::number(ax, 'f', 2) << QString::number(ay, 'f', 2) << QString::number(az, 'f', 2)
                 << QString::number(gx, 'f', 2) << QString::number(gy, 'f', 2) << QString::number(gz, 'f', 2)
                 << QString::number(mx, 'f', 2) << QString::number(my, 'f', 2) << QString::number(mz, 'f', 2)
                 << QString::number(headingDeg)
                 << QString::number(pressure)
                 << QString::number(temperature);

        emit sig_updateUI(dataList);
    };
    QThreadPool::globalInstance()->start(runnable);
}
