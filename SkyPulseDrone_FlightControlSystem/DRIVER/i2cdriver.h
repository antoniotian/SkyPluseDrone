/**
 * @file i2cdriver.h
 * @brief Generic classes for i2c communication
 *
 */
#ifndef I2CDRIVER_H
#define I2CDRIVER_H

#include <QObject>
#include <QMutex>
#include <QDebug>
#include <QMutexLocker>
#include <QByteArray>
#include <pigpio.h>

/**
 * @class i2cDriver
 * @brief Generic class for i2c communication
 * Provide i2c communication between sensors and Raspberry Pi
 * includes read/write single byte, and read/write multiple bytes.
 *
 */
class I2CDevice : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief i2cDriver constructor
     * @param deviceAddress
     * @param parent
     *
     */
    explicit I2CDevice(int deviceAddress, QObject *parent = nullptr);

    /**
     * @brief i2cDriver destructor
     * Release all i2c resources.
     *
     */
    ~I2CDevice();

    /**
     * @brief initialize
     * Initializing the i2c Configuration
     * @return bool
     *
     */
    bool initialize();

    /**
     * @brief readByte
     * @param reg
     * @param value
     * @return
     */
    bool readByte(quint8 reg, quint8 &value);

    /**
     * @brief readBytes
     * @param registerAddress
     * @param count
     * @return
     */
    QByteArray readBytes(quint8 registerAddress, quint8 count);

    /**
     * @brief writeByte
     * @param reg
     * @param value
     * @return
     */
    bool writeByte(quint8 reg, quint8 value);

    /**
     * @brief writeBytes
     * @param registerAddress
     * @param data
     * @return
     */
    bool writeBytes(quint8 registerAddress, const QByteArray &data);

private:
    int deviceAddress;
    int handle;  // Handle returned by the i2cOpen function
    static QMutex mutex;

};

#endif // I2CDRIVER_H
