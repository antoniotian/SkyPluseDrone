#include "sensormanager.h"

SensorManager::SensorManager(QObject *parent)
    : QObject{parent}
{

}
SensorManager::~SensorManager()
{
    delete MPU6050_HANDLE;
    delete BMP180_HANDLE;
    delete QMC5883_HANDLE;
}

void SensorManager::initialize()
{
    MPU6050_HANDLE = new MPU6050(MPU6059_DEVICE_ADDR, this);
    BMP180_HANDLE  = new BMP180(BMP180_DEVICE_ADDR, this);
    QMC5883_HANDLE = new QMC5883(QMC5883_DEVICE_ADDR, this);
}


void SensorManager::ReadAllSensorData()
{
    QtConcurrent::run([this]() {
        qDebug() << "Starting sensor read operation in a separate thread" << QThread::currentThreadId();
        float ax, ay, az, gx, gy, gz, temperature, pressure, mx, my, mz, headingDeg;

        MPU6050_HANDLE->readAllSensors(ax, ay, az, gx, gy, gz);
        BMP180_HANDLE->readTemperature(temperature);
        BMP180_HANDLE->readPressure(pressure);
        QMC5883_HANDLE->readRawData(mx, my, mz, headingDeg);

        QVector<float> AccGyr = {ax, ay, az, gx, gy, gz, mx, my, mz, headingDeg, pressure, temperature};
        packageAndSendSensorData(AccGyr);
        emit updateSensorData(AccGyr);
    });
}

void SensorManager::packageAndSendSensorData(const QVector<float> &AccGry)
{
    QByteArray buffer;
    QDataStream stream(&buffer, QIODevice::WriteOnly);
    stream.setByteOrder(QDataStream::LittleEndian);
    for (float value : AccGry) {
         stream << value;  // Write each floating point number to the stream
     }
    // If you need to ensure the data size is 64 bytes, you can add padding here
    while (buffer.size() < 64) {
        stream << static_cast<quint8>(0);  // Pad to 64 bytes with 0s
    }
    emit sig_sendMessage64Bytes(buffer);  // Send signals
}
