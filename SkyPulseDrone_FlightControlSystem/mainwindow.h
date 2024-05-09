#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QThread>
#include <QDebug>
#include <QTimer>

#include "sensormanager.h"

/*COMM Head Files*/
#include "tcp.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_Function_01_clicked();

private:
    Ui::MainWindow *ui;

    SensorManager *sensor_manager;

    /* Network Protocol */
    TCP *tcp_server;

    /* Timer */
    QTimer *readTimer;
    bool timerRunning = false;

signals:
    void sig_readAllSensorData();

};
#endif // MAINWINDOW_H
