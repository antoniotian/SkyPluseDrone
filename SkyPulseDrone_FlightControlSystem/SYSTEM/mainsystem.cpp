#include "mainsystem.h"

MainSystem::MainSystem(QWidget *parent)
    : QWidget{parent}
{
    qDebug() << "Main Thread:" << QThread::currentThreadId();
    initialUI();
    initialGPIO();
    initialSystem();
    initialNetwork();
}

MainSystem::~MainSystem()
{
    qDebug() << "System Quit";
    delete readTimer;
    gpioTerminate();
}

void MainSystem::initialUI()
{
    // Create Pushbutton
    QPushButton *function_button1 = new QPushButton("Function1", this);

    // Adding buttons to a layout
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(function_button1);

    setLayout(buttonLayout);

    // Connecting signals and slots
    connect(function_button1, &QPushButton::clicked, this, &MainSystem::onButtonClicked_Function1);
}

void MainSystem::initialNetwork()
{
    tcp_server = new TCP();
    connect(sensor_manager, &SensorManager::sig_sendMessage64Bytes, tcp_server, &TCP::sendMessage64Bytes);
}


void MainSystem::initialGPIO()
{
    // Initial GPIO
    while(true){
        if (gpioInitialise() < 0) {
            qDebug() << "Failed to initialize pigpio. Retrying in 1 second...";
            QThread::sleep(2); // wait for 1 s
            continue; // continue loop
        }
        qDebug() << "Pigpio initialized successfully";
        break; // Successful initialization, jump out of the loop
    }
}


void MainSystem::initialSystem()
{
    sensor_manager = new SensorManager();
    readTimer = new QTimer(this);

    sensor_manager->initialize();
    connect(readTimer, &QTimer::timeout, sensor_manager, &SensorManager::ReadAllSensorData);
    connect(this, &MainSystem::sig_readAllSensorData, sensor_manager, &SensorManager::ReadAllSensorData);
    connect(sensor_manager, &SensorManager::updateSensorData, this, [](QVector<float> AccGyr){
            qDebug() << "Acceleration:" << AccGyr[0] << "," << AccGyr[1] << "," << AccGyr[2];
            qDebug() << "Gyroscope:" << AccGyr[3] << "," << AccGyr[4] << "," << AccGyr[5];
            qDebug() << "Magnetometer:" << AccGyr[6] << "," << AccGyr[7] << "," << AccGyr[8];
            qDebug() << "Heading Degree:" << AccGyr[9];
            qDebug() << "Pressure:" << AccGyr[10];
            qDebug() << "Temperture:" << AccGyr[11];
        });
}


void MainSystem::onButtonClicked_Function1()
{
    // emit sig_readAllSensorData();
    if (timerRunning) {
        readTimer->stop();
        qDebug() << "Timer stopped";
    }
    else {
        readTimer->start(100);
        qDebug() << "Timer started";
    }
    timerRunning = !timerRunning;
}
