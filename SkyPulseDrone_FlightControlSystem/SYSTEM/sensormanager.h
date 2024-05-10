#ifndef SENSORMANAGER_H
#define SENSORMANAGER_H

#include <QObject>
#include <QVector>
#include <QThreadPool>
#include <QtConcurrent/QtConcurrent>

#include "mpu6050.h"
#include "bmp180.h"
#include "qmc5883.h"

#define MPU6059_DEVICE_ADDR 0x68
#define BMP180_DEVICE_ADDR 0x77
#define QMC5883_DEVICE_ADDR 0x0D

class SensorManager : public QObject
{
    Q_OBJECT

public:
    explicit SensorManager(QObject *parent = nullptr);
    ~SensorManager();

    void initialize();

public slots:
    void ReadAllSensorData();

private:
    MPU6050 *MPU6050_HANDLE;
    BMP180  *BMP180_HANDLE;
    QMC5883 *QMC5883_HANDLE;

    void packageAndSendSensorData(const QVector<float> &AccGry);

signals:
    void updateSensorData(const QVector<float> &AccGry);
    void sig_sendMessage64Bytes(QByteArray data_buffer);

};

#endif // SENSORMANAGER_H
