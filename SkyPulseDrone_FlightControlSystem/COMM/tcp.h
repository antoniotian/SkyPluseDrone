#ifndef TCP_H
#define TCP_H

#include <QObject>
#include <QVector>
#include <QTcpServer>
#include <QTcpSocket>
#include <QTimer>
#include <QThread>
#include <QHostAddress>
#include <QThreadPool>
#include <QDataStream>
#include <QDebug>

class TCP : public QTcpServer
{
    Q_OBJECT

public:
    explicit TCP(QObject *parent = nullptr);
    ~TCP();

public slots:
    void startServer();
    void stopServer();
    void broadcastMessage(const QByteArray &message);
    void sendMessage64Bytes(const QByteArray &datapackage_u64);

private:
    QList<QTcpSocket*> clients;
    QThread *TCPThread;

    static constexpr quint16 PORT = 12345;

private slots:
    void onReadyRead();

protected:
    void incomingConnection(qintptr socketDescriptor) override;

signals:

};

#endif // TCP_H
