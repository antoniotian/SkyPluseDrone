#include "bmp180.h"

BMP180::BMP180(uint8_t i2cAddress, QObject *parent)
    : QObject(parent), i2cDevice(new I2CDevice(i2cAddress, this))
{
    qWarning("BMP180 Initial");
    if(!i2cDevice->initialize()){
        qWarning("Failed to create I2C device for BMP180");
        return;
    }
    if(initialize()){
        qWarning("BMP180 Config Successful");
    }
    else{
        qWarning("BMP180 Config Failed");
    }
}


// Run calibration on initialisation and read calibration values
bool BMP180::initialize()
{
    if(!readCalibrationData()){
        return false;
    }
    startPressureMeasurement(OSS);
    return true;
}


bool BMP180::readCalibrationData()
{
    if(!i2cDevice){
        return false;
    }
    // Read calibration data
    if (!readShortFromRegister(0xAA, AC1) || !readShortFromRegister(0xAC, AC2) ||
        !readShortFromRegister(0xAE, AC3) || !readShortFromRegister(0xB0, AC4) ||
        !readShortFromRegister(0xB2, AC5) || !readShortFromRegister(0xB4, AC6) ||
        !readShortFromRegister(0xB6, B1)  || !readShortFromRegister(0xB8, B2)  ||
        !readShortFromRegister(0xBA, MB)  || !readShortFromRegister(0xBC, MC)  ||
        !readShortFromRegister(0xBE, MD))
    {
        qWarning("Error reading calibration data from BMP180");
        return false;
    }
    return true;
}


bool BMP180::startTemperatureMeasurement()
{
    const quint8 temperatureCmd = 0x2E;
    const quint8 controlReg = 0xF4;
    if (!i2cDevice->writeByte(controlReg, temperatureCmd)) {
        qWarning("Failed to initiate temperature measurement");
        return false;
    }
    QThread::msleep(5);  // Make sure there is enough time for data conversion
    return true;
}


int16_t BMP180::readRawTemperature ()
{
    if (!startTemperatureMeasurement()) return 0;

    quint8 msb = 0, lsb = 0;
    const quint8 tempDataReg = 0xF6;
    if((!i2cDevice->readByte(tempDataReg, msb)) || (!i2cDevice->readByte(tempDataReg + 1, lsb))) {
        qWarning("Read raw temperature reg error");
        return 0;
    }
    return (msb << 8) | lsb;
}


void BMP180::readTemperature(float &temperature)
{
    short rawTemp = readRawTemperature();
    if (rawTemp == 0) return;

    // Calculate actual temperature
    long x1 = (rawTemp - (long)AC6) * (long)AC5 / 32768;
    long x2 = ((long)MC * 2048) / (x1 + MD);
    long b5 = x1 + x2;
    temperature = (b5 + 8) / 16.0f / 10.0f;
}


void BMP180::startPressureMeasurement(int oss)
{
    const quint8 controlReg = 0xF4;
    quint8 cmd = 0x34 + (oss << 6);
    i2cDevice->writeByte(controlReg, cmd);
}


void BMP180::readPressure(float &pressure)
{
    if (!i2cDevice) {
        qWarning("I2C device is not initialized");
        return;
    }
    quint8 reg = 0xF6;
    quint8 count = 3;
    QByteArray rawData = i2cDevice->readBytes(reg, count);

    if (rawData.size() == count) {
        int rawPressure = (static_cast<unsigned char>(rawData[0])  << 16 |
                           static_cast<unsigned char>(rawData[1])  << 8  |
                           static_cast<unsigned char>(rawData[2])) >> (8 /* oversampling_setting */);
        pressure = static_cast<float>(calculatePressure(rawPressure));
    } else {
        qWarning("Failed to read pressure data from BMP180");
        return;
    }
}


double BMP180::calculatePressure(int rawPressure)
{
    int32_t B5;
    int32_t UT = readRawTemperature();
    int32_t X1 = (UT - (int32_t)AC6) * ((int32_t)AC5) / pow(2, 15);
    int32_t X2 = ((int32_t)MC * pow(2, 11)) / (X1 + (int32_t)MD);
    B5 = X1 + X2;

    // Calculate B6, B3, X1, X2, X3, B4, B7
    int32_t B6 = B5 - 4000;
    X1 = (B2 * (B6 * B6 / pow(2, 12))) / pow(2, 11);
    X2 = AC2 * B6 / pow(2, 11);
    int32_t X3 = X1 + X2;
    int32_t B3 = (((AC1 * 4 + X3) << OSS) + 2) / 4;

    X1 = AC3 * B6 / pow(2, 13);
    X2 = (B1 * (B6 * B6 / pow(2, 12))) / pow(2, 16);
    X3 = ((X1 + X2) + 2) / 4;
    uint32_t B4 = AC4 * (uint32_t)(X3 + 32768) / pow(2, 15);
    uint32_t B7 = ((uint32_t)rawPressure - B3) * (50000 >> OSS);

    int32_t p;
    if (B7 < 0x80000000) {
        p = (B7 * 2) / B4;
    } else {
        p = (B7 / B4) * 2;
    }

    X1 = (p / pow(2, 8)) * (p / pow(2, 8));
    X1 = (X1 * 3038) / pow(2, 16);
    X2 = (-7357 * p) / pow(2, 16);
    p = p + (X1 + X2 + 3791) / pow(2, 4);

    return p;
}


bool BMP180::readShortFromRegister(uint8_t registerAddress, short &value)
{
    QByteArray data = i2cDevice->readBytes(registerAddress, 2);
    if (data.size() == 2) {
        value = static_cast<short>((data[0] << 8) | data[1]);
        return true;
    }
    return false;
}
