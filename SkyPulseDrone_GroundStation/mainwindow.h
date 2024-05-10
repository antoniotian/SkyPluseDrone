#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QThread>
#include <QDebug>
#include <QNetworkInterface>
#include <QCloseEvent>

#include "tcp.h"
#include "udp.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


public slots:
    void onTCPStartSuccessful();
    void onTCPStopSuccessful();
    void onTCPError();

    void onUDPStartSuccessful();
    void onUDPStopSuccessful();
    void onUDPError();

    void updateUI(const QStringList &datalist);


private:
    Ui::MainWindow *ui;

    /* Network Protocol Instance*/
    TCP *TCPServer;
    UDP *UDPServer;

    void cleanUI();

private slots:
    void on_pushButton_Connect_clicked();
    void on_pushButton_Disconnect_clicked();


signals:
    /* TCP&UDP Server Signals*/
    void sig_StartTCPServer(const QString &ip_address, const quint16 &port);
    void sig_StartUDPServer(const quint16 &port);
    void sig_StopTCPServer();
    void sig_StopUDPServer();
};
#endif // MAINWINDOW_H
