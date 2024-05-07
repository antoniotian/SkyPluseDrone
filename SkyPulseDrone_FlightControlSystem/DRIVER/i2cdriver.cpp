#include "i2cdriver.h"

QMutex i2cDriver::mutex;


i2cDriver::i2cDriver(int deviceAddress, QObject *parent)
    : QObject(parent), deviceAddress(deviceAddress), handle(-1)
{

}

i2cDriver::~i2cDriver()
{
    if(handle >= 0){  //
        i2cClose(handle);
    }
}

bool i2cDriver::initialize()
{
    QMutexLocker locker(&mutex);  //

    if (gpioInitialise() < 0) {
        qWarning("Failed to initialize pigpio. Retrying in 1 second...");
    }
    qWarning("Pigpio initialized successfully");

    handle = i2cOpen(1, deviceAddress, 0);  //
    if(handle < 0){
        qDebug() << "Failed to open I2C device : " << deviceAddress;
        return false;
    }
    qDebug() << "I2C device opened: " << QString::number(deviceAddress, 16);
    return true;
}

QByteArray i2cDriver::readBytes(quint8 registerAddress, quint8 count)
{
    QByteArray data(count, 0);  //
}
