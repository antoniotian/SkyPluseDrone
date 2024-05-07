#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    sensor_manager = new SensorManager();
    sensor_manager->initialize();
}

MainWindow::~MainWindow()
{
    qWarning("System Quit");
    delete ui;
    gpioTerminate();
}

