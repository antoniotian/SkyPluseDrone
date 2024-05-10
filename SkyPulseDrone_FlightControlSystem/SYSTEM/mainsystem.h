#ifndef MAINSYSTEM_H
#define MAINSYSTEM_H

#include <QWidget>
#include <QObject>
#include <QDebug>
#include <QThread>
#include <QTimer>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <pigpio.h>

#include "tcp.h"
#include "sensormanager.h"

class MainSystem : public QWidget
{
    Q_OBJECT

public:
    explicit MainSystem(QWidget *parent = nullptr);
    ~MainSystem();

private:
    /* Network Protocol */
    TCP *tcp_server;

    /* Timer */
    QTimer *readTimer;
    bool timerRunning = false;

    SensorManager *sensor_manager;

    void initialUI();
    void initialNetwork();
    void initialGPIO();
    void initialSystem();

private slots:
    void onButtonClicked_Function1();

signals:
    void sig_readAllSensorData();
};

#endif // MAINSYSTEM_H
