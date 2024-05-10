#include "i2cdriver.h"

QMutex I2CDevice::mutex;

I2CDevice::I2CDevice(int deviceAddress, QObject *parent)
    : QObject(parent), deviceAddress(deviceAddress), handle(-1)
{

}


I2CDevice::~I2CDevice()
{
    if(handle >= 0){  // Check the handle exist
        i2cClose(handle);
    }
}


bool I2CDevice::initialize()
{
    QMutexLocker locker(&mutex);  // Thread Mutex locker, Ensure that the thread is accessed one task at a time

    handle = i2cOpen(1, deviceAddress, 0);  // Generate handle by specifying the device address
    if(handle < 0){
        qDebug() << "Failed to open I2C device : " << deviceAddress;
        return false;
    }
    qDebug() << "I2C device opened: " << QString::number(deviceAddress, 16);
    return true;
}


bool I2CDevice::readByte(quint8 reg, quint8 &value)
{
    QMutexLocker locker(&mutex);

    int result = i2cReadByteData(handle, reg);
    if(result >= 0){
        value = result;
        return true;
    }
    return false;
}


QByteArray I2CDevice::readBytes(quint8 registerAddress, quint8 count)
{
    QByteArray data(count, 0);  // Automating Memory Management with Qt Containers
    {
        QMutexLocker locker(&mutex);
        if (i2cReadI2CBlockData(handle, registerAddress, data.data(), count) == count) {
            return data;
        }
        else {
            qDebug() << "Failed to read from I2C device at register:" << registerAddress;
            return QByteArray();
        }
    }
}


bool I2CDevice::writeByte(quint8 reg, quint8 value)
{
    QMutexLocker locker(&mutex);
    int result = i2cWriteByteData(handle, reg, value);
    if(result >= 0){
        return true;
    }
    return false;
}


bool I2CDevice::writeBytes(quint8 registerAddress, const QByteArray &data)
{
    QMutexLocker locker(&mutex);
    if(i2cWriteI2CBlockData(handle, registerAddress, const_cast<char*>(data.data()), data.size()) == 0){
        return true;
    }
    else{
        qDebug() << "Failed to write to I2C device." << registerAddress;
        return false;
    }
}
