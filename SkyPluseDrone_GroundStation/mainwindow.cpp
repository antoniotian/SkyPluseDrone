#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->progressBar->setRange(0, 0);
    ui->progressBar->hide();

    TCPServer = new TCP();
    UDPServer = new UDP();

    /* TCP Server Connection Signals */
    connect(this, &MainWindow::sig_StartTCPServer,  TCPServer, &TCP::startTCPServer);
    connect(this, &MainWindow::sig_StopTCPServer,   TCPServer, &TCP::stopTCPServer);
    connect(this, &MainWindow::sig_StartUDPServer,  UDPServer, &UDP::startUDPServer);
    connect(this, &MainWindow::sig_StopUDPServer,   UDPServer, &UDP::stopUDPServer);

    connect(TCPServer, &TCP::sig_serverStarted, this, &MainWindow::onTCPStartSuccessful);
    connect(TCPServer, &TCP::sig_serverStopped, this, &MainWindow::onTCPStopSuccessful);
    connect(TCPServer, &TCP::sig_tcpError, this, &MainWindow::onTCPError);

    connect(UDPServer, &UDP::sig_serverStarted, this, &MainWindow::onUDPStartSuccessful);
    connect(UDPServer, &UDP::sig_serverStopped, this, &MainWindow::onUDPStopSuccessful);
    connect(UDPServer, &UDP::sig_udpError, this, &MainWindow::onUDPError);

    connect(TCPServer, &TCP::sig_updateUI, this, &MainWindow::updateUI);
}

MainWindow::~MainWindow()
{
    delete ui;
}

/*========================================================================
* TCP & UDP Server
*=========================================================================*/
void MainWindow::on_pushButton_Connect_clicked()
{
    bool ok;
    QString NetworkIPAddr = ui->lineEdit_IP_Address->text();
    quint16 NetworkPort = ui->lineEdit_Port->text().toInt(&ok,10);

    // 判断端口输入是否有效
    if(ok){
        emit sig_StartTCPServer(NetworkIPAddr, NetworkPort);  // 采用信号的方式进行连接
        emit sig_StartUDPServer(NetworkPort);  // 开启UDP服务
        ui->progressBar->show();
    }
    else{
        QMessageBox::critical(this, "Error", "Error Port Input");
        ui->lineEdit_Port->clear();
    }
}

void MainWindow::on_pushButton_Disconnect_clicked()
{
    emit sig_StopTCPServer();
    emit sig_StopUDPServer();
}

void MainWindow::onTCPStartSuccessful()
{
    // 处理连接成功事件，更新UI状态
    ui->progressBar->hide();
    ui->pushButton_Connect->setEnabled(false);
    ui->pushButton_Disconnect->setEnabled(true);
    ui->icon_TCP->setPixmap(QPixmap(":/icon/yes.png"));
}

void MainWindow::onTCPStopSuccessful()
{
    // 处理连接失败事件，更新UI状态
    ui->progressBar->hide();
    ui->pushButton_Connect->setEnabled(true);
    ui->pushButton_Disconnect->setEnabled(false);
    ui->icon_TCP->setPixmap(QPixmap(":/icon/no.png"));
    cleanUI();
}

void MainWindow::onTCPError()
{
    // 处理连接失败事件，例如更新UI状态
    ui->progressBar->hide();
    ui->pushButton_Connect->setEnabled(true);
    ui->pushButton_Disconnect->setEnabled(false);
    QMessageBox::critical(this, "Error", "TCP Connection Error. Please check the client device and try again.");
    ui->icon_TCP->setPixmap(QPixmap(":/icon/no.png"));
}

void MainWindow::onUDPStartSuccessful()
{
    ui->icon_UDP->setPixmap(QPixmap(":/icon/yes.png"));
}

void MainWindow::onUDPStopSuccessful()
{
    ui->icon_UDP->setPixmap(QPixmap(":/icon/no.png"));
}

void MainWindow::onUDPError()
{
    ui->progressBar->hide();
    ui->pushButton_Connect->setEnabled(true);
    ui->pushButton_Disconnect->setEnabled(false);
    QMessageBox::critical(this, "Error", "UDP Connection Error. Please check the client device and try again.");
    ui->icon_UDP->setPixmap(QPixmap(":/icon/no.png"));
}

void MainWindow::updateUI(const QStringList &datalist)
{
    QVector<QLineEdit*> Sensor_lineEdits {
        ui->lineEdit_ACC_X, ui->lineEdit_ACC_Y, ui->lineEdit_ACC_Z,
        ui->lineEdit_GYR_X, ui->lineEdit_GYR_Y, ui->lineEdit_GYR_Z,
        ui->lineEdit_MAG_X, ui->lineEdit_MAG_Y, ui->lineEdit_MAG_Z,
        ui->lineEdit_HeadDeg, ui->lineEdit_Altitude, ui->lineEdit_Temp
    };

    for (int i = 0; i < datalist.size(); i++)
    {
        Sensor_lineEdits[i]->setText(datalist.at(i));
    }
}

void MainWindow::cleanUI()
{
    QVector<QLineEdit*> Sensor_lineEdits {
        ui->lineEdit_ACC_X, ui->lineEdit_ACC_Y, ui->lineEdit_ACC_Z,
        ui->lineEdit_GYR_X, ui->lineEdit_GYR_Y, ui->lineEdit_GYR_Z,
        ui->lineEdit_MAG_X, ui->lineEdit_MAG_Y, ui->lineEdit_MAG_Z,
        ui->lineEdit_HeadDeg, ui->lineEdit_Altitude, ui->lineEdit_Temp
    };

    for (int i = 0; i < Sensor_lineEdits.size(); i++)
    {
        Sensor_lineEdits[i]->clear();
    }
}
