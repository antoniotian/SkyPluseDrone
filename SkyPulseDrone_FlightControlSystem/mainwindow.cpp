#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    qDebug() << "The main thread is: " << QThread::currentThreadId();

    // Initial GPIO
    while(true){
        if (gpioInitialise() < 0) {
            qWarning("Failed to initialize pigpio. Retrying in 1 second...");
            QThread::sleep(2); // wait for 1 s
            continue; // continue loop
        }
        qWarning("Pigpio initialized successfully");
        break; // Successful initialization, jump out of the loop
    }

    sensor_manager = new SensorManager();
    sensor_manager->initialize();
    connect(this, &MainWindow::sig_readAllSensorData, sensor_manager, &SensorManager::ReadAllSensorData);
    connect(sensor_manager, &SensorManager::updateSensorData, this, [](QVector<float> AccGyr){
            qDebug() << "Acceleration:" << AccGyr[0] << "," << AccGyr[1] << "," << AccGyr[2];
            qDebug() << "Gyroscope:" << AccGyr[3] << "," << AccGyr[4] << "," << AccGyr[5];
            qDebug() << "Magnetometer:" << AccGyr[6] << "," << AccGyr[7] << "," << AccGyr[8];
            qDebug() << "Heading Degree:" << AccGyr[9];
            qDebug() << "Pressure:" << AccGyr[10];
            qDebug() << "Temperture:" << AccGyr[11];
        });

    tcp_server = new TCP();
    connect(sensor_manager, &SensorManager::sig_sendMessage64Bytes, tcp_server, &TCP::sendMessage64Bytes);

    readTimer = new QTimer(this);
    connect(readTimer, &QTimer::timeout, sensor_manager, &SensorManager::ReadAllSensorData);
}


MainWindow::~MainWindow()
{
    qWarning("System Quit");
    delete ui;
    gpioTerminate();
}


void MainWindow::on_pushButton_Function_01_clicked()
{
//    emit sig_readAllSensorData();
    if (timerRunning) {
        readTimer->stop();
        qDebug() << "Timer stopped";
    } else {
        readTimer->start(100);
        qDebug() << "Timer started";
    }
    timerRunning = !timerRunning;
}

