#include "sensormanager.h"

SensorManager::SensorManager(QObject *parent)
    : QObject{parent}
{

}
SensorManager::~SensorManager()
{

}

void SensorManager::initialize()
{
    MPU6050_HANDLE = new MPU6050(MPU6059_DEVICE_ADDR, this);
}
