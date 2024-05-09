#ifndef UDP_H
#define UDP_H

#include <QObject>
#include <QThread>
#include <QUdpSocket>

class UDP : public QObject
{
    Q_OBJECT

public:
    explicit UDP(QObject *parent = nullptr);
    ~UDP();

public slots:
    void startUDPServer(quint16 port);
    void stopUDPServer();

private:
    QUdpSocket *udpSocket;
    QThread *udpThread;

private slots:
    void udpInitial();
    void onConnected();
    void onDisconnected();
    void onErrorOccurred();
    void readPendingDatagrams();

signals:
    void sig_messageReceived(const QString &message);
    void sig_serverStarted();
    void sig_serverStopped();
    void sig_udpError();
};

#endif // UDP_H
