#ifndef QMC5883_H
#define QMC5883_H

#include <QObject>
#include <QDebug>
#include <QThread>
#include <cmath>
#include "i2cdriver.h"

class QMC5883 : public QObject
{
    Q_OBJECT
public:
    explicit QMC5883(uint8_t i2cAddress = 0x0D, QObject *parent = nullptr);
    bool initialize();

public slots:
    float readHeading(const float &x, const float &y);
    void readRawData(float &x, float &y, float &z, float &headingDegree);

private:
    I2CDevice *i2cDevice;
    float convertToRawData(const QByteArray &bytes, int offset);

};

#endif // QMC5883_H
