#include "kalmanfilter.h"

KalmanFilter::KalmanFilter(int stateSize, int measSize, int controlSize, double dt, QObject *parent)
    : QObject(parent),
      A(stateSize, stateSize),
      B(stateSize, controlSize),
      H(measSize, stateSize),
      Q(stateSize, stateSize),
      R(measSize, measSize),
      P(stateSize, stateSize),
      x(stateSize),
      dt(dt)
{
    A.setIdentity();
    B.setZero();
    H.setZero();
    Q.setZero();
    R.setZero();
    P.setZero();
    x.setZero();
}


void KalmanFilter::predict(const Eigen::VectorXd &u) {
    x = A * x + B * u;
    P = A * P * A.transpose() + Q;
}


void KalmanFilter::update(const Eigen::VectorXd &z) {
    Eigen::MatrixXd K = P * H.transpose() * (H * P * H.transpose() + R).inverse();
    x = x + K * (z - H * x);
    P = (Eigen::MatrixXd::Identity(x.size(), x.size()) - K * H) * P;
}


Eigen::VectorXd KalmanFilter::getState() const
{
    return x;
}
