#include "mpu6050.h"

MPU6050::MPU6050(quint8 i2cAddress, QObject *parent)
    : QObject(parent), I2CDriver(new i2cDriver(i2cAddress, this))
{
    qWarning("MPU6050 Initial");
    if(!I2CDriver->initialize()){
        qWarning("Failed to create I2C device for MPU6050");
        return;
    }
//    if(initialize()){
//        qWarning("MPU6050 Config Successful");
//    }
//    else{
//        qWarning("MPU6050 Config Failed");
//    }
}
