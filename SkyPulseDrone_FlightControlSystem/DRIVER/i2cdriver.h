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
class i2cDriver : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief i2cDriver constructor
     * @param deviceAddress
     * @param parent
     *
     */
    explicit i2cDriver(int deviceAddress, QObject *parent = nullptr);

    /**
     * @brief i2cDriver destructor
     * Release all i2c resources.
     *
     */
    ~i2cDriver();

    /**
     * @brief initialize
     * Initializing the i2c Configuration
     * @return bool
     *
     */
    bool initialize();

    /**
     * @brief readBytes
     * @param registerAddress
     * @param count
     * @return
     */
    QByteArray readBytes(quint8 registerAddress, quint8 count);

    /**
     * @brief readByte
     * @param reg
     * @param value
     * @return
     */
    bool readByte(quint8 reg, quint8 &value);

    /**
     * @brief writeBytes
     * @param registerAddress
     * @param data
     * @return
     */
    bool writeBytes(quint8 registerAddress, const QByteArray &data);

    /**
     * @brief writeByte
     * @param reg
     * @param value
     * @return
     */
    bool writeByte(quint8 reg, quint8 value);

private:
    int deviceAddress;
    int handle;
    static QMutex mutex;

signals:
    void errorOccurred(const QString &message);

};

#endif // I2CDRIVER_H
