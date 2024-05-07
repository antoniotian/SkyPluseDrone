#ifndef MPU6050_H
#define MPU6050_H

#include <QObject>
#include <QThread>
#include "i2cdriver.h"

class MPU6050 : public QObject
{
    Q_OBJECT

public:
    explicit MPU6050(quint8 i2cAddress = 0x68, QObject *parent = nullptr);
//    bool calibrateGyro();
//    bool calbrateAccel();
//    bool readAllSensors(float &ax, float &ay, float &az, float &gx, float &gy, float &gz);
//    bool readBytes(quint8 reg, quint8 *buffer, size_t length);

public slots:
//    bool readAllReg();

private:
    i2cDriver *I2CDriver;

    // Calibration offsets
    float gyroOffset[3]  = {0, 0, 0};
    float accelOffset[3] = {0, 0, 0};

    static constexpr uint8_t PWR_MGMT_1         = 0x6B;
    static constexpr uint8_t ACCEL_XOUT_H       = 0x3B;
    static constexpr uint8_t GYRO_XOUT_H        = 0x43;
    static constexpr uint8_t INT_ENABLE         = 0x38;  // Interrupt Enable Register
    static constexpr uint8_t INT_PIN_CFG        = 0x37;  // Interrupt Pin/Bypass Enable Configuration Register
    static constexpr float   ACCEL_FS_SEL_2G    = 16384.0;
    static constexpr float   GYRO_FS_SEL_250DEG = 131.0;

    bool initialize();
//    void collectSensorDataForCalibration();
//    void applyCalibration(float &ax, float &ay, float &az, float &gx, float &gy, float &gz);

signals:

};

#endif // MPU6050_H
