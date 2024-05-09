#include "qmc5883.h"

QMC5883::QMC5883(uint8_t i2cAddress, QObject *parent)
    : QObject(parent), i2cDevice(new I2CDevice(i2cAddress, this))
{
    if (!i2cDevice->initialize()){
        qWarning("Failed to create I2C device for QMC5883");
        return;
    }
    if (this->initialize()){
        qWarning("QMC5883 Config Successful");
    }
    else{
        qWarning("QMC5883 Config Failed");
    }
}


bool QMC5883::initialize()
{
    /*Configuration register*/
    if (!i2cDevice->writeByte(0x0B, 0x01) // Configure register A for 8 average, 15Hz default, normal measurement
        || !i2cDevice->writeByte(0x09, 0x1D)) { // Configuration Gain
        qWarning("Failed to write configuration to Magnetometer QMC5883");
        return false;
    }
    return true;
}


float QMC5883::readHeading(const float &x, const float &y)
{
    float heading = atan2(y, x) * 180.0 / M_PI; // Calculate the angle in radians and Convert to degrees
    // Normalize to 0-360 degrees
    if (heading < 0) {
        heading += 360.0;
    }
    return heading;
}


void QMC5883::readRawData(float &x, float &y, float &z, float &headingDegree)
{
    QByteArray data = i2cDevice->readBytes(0x00, 6);
    if (data.size() == 6) {
        x = convertToRawData(data, 0);
        y = convertToRawData(data, 2);
        z = convertToRawData(data, 4);
        headingDegree = readHeading(x, y);
    }
    else{
        qWarning("Failed to read data from the device");
    }
}



float QMC5883::convertToRawData(const QByteArray& bytes, int offset)
{
    int16_t value = static_cast<int16_t>((bytes[offset] << 8) | (bytes[offset + 1] & 0xFF));
    return static_cast<float>(value);;
}
