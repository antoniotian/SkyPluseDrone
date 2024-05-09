#ifndef TCP_H
#define TCP_H

#include <QDebug>
#include <QObject>
#include <QTcpSocket>
#include <QThread>
#include <QThreadPool>
#include <QtConcurrent/QtConcurrent>

class TCP : public QObject
{
    Q_OBJECT

public:
    explicit TCP(QObject *parent = nullptr);
    ~TCP();

    /**
     * @brief startTCPServer
     * @param host_ip
     * @param port
     */
    void startTCPServer(const QString &host_ip, quint16 port);

    /**
     * @brief stopTCPServer
     */
    void stopTCPServer();

public slots:
    /**
     * @brief sendMessage
     * @param message
     */
    void sendMessage(const QByteArray &message);

    /**
     * @brief readMessage
     */
    void readMessage();

private:
    QTcpSocket *TCPSocket;
    QThread *TCPThread;

    void dataDecoder(const QByteArray &data_to_decode);

private slots:
    void tcpInitial();
    void onConnected();
    void onDisconnected();
    void onErrorOccurred();

signals:
    void sig_serverStarted();
    void sig_serverStopped();
    void sig_tcpError();
    void sig_updateUI(const QStringList &data);
};

#endif // TCP_H
