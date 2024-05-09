#ifndef KALMANFILTER_H
#define KALMANFILTER_H

#include <QObject>
#include <Eigen/Dense>

class KalmanFilter : public QObject
{
    Q_OBJECT
public:
    explicit KalmanFilter(int stateSize, int measSize, int controlSize, double dt, QObject *parent = nullptr);

    void predict(const Eigen::VectorXd &u);
    void update(const Eigen::VectorXd &z);

    Eigen::VectorXd getState() const;

private:
    Eigen::MatrixXd A; // state transition matrix
    Eigen::MatrixXd B; // control input matrix
    Eigen::MatrixXd H; // observation matrix
    Eigen::MatrixXd Q; // Process noise covariance matrix
    Eigen::MatrixXd R; // Observation noise covariance matrix
    Eigen::MatrixXd P; // Estimated error covariance
    Eigen::VectorXd x; // state vector

    double dt; // sampling time

};

#endif // KALMANFILTER_H
