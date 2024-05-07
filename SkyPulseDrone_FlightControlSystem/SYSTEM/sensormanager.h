#ifndef SENSORMANAGER_H
#define SENSORMANAGER_H

#include <QObject>
#include <QThreadPool>
#include <QtConcurrent/QtConcurrent>

#include "mpu6050.h"

#define MPU6059_DEVICE_ADDR 0x68

class SensorManager : public QObject
{
    Q_OBJECT

public:
    explicit SensorManager(QObject *parent = nullptr);
    ~SensorManager();

    void initialize();

private:
    MPU6050 *MPU6050_HANDLE;

signals:

};

#endif // SENSORMANAGER_H
