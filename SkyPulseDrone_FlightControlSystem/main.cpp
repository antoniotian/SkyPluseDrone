#include "mainwindow.h"

#include <QApplication>
#include <QSplashScreen>
#include <QProgressBar>
#include <QTimer>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    qRegisterMetaType<QVector<float>>("QVector<float>");

    /*Creating the Startup Screen*/
    QSplashScreen *splash = new QSplashScreen;
    splash->setPixmap(QPixmap(":/Resource/Background.jpg"));
    splash->show();

    /*Adding a Progress Bar to the Startup Screen*/
    QProgressBar *progressBar = new QProgressBar(splash);
    progressBar->setGeometry(0, splash->height() - 20, splash->width(), 20);  // Setting the progress bar position and size
    progressBar->setValue(0);
    progressBar->show();

    /*Main Window Display*/
    MainWindow mainWindow;

    /*Simulate Loading Process*/
    QTimer *timer = new QTimer(splash);
    int progress = 0;
    QObject::connect(timer, &QTimer::timeout, [&]() {
            progress += 10; // Increase by 1% each time
            progressBar->setValue(progress);

            if (progress >= 100) {
                timer->stop();  // Stop timer
                splash->finish(&mainWindow); // Complete the startup task, close the splash screen and pass the main window pointer
                mainWindow.show();  // Show main window
                splash->deleteLater(); // Clean up resources
            }
        });

    timer->start(50);
    return app.exec();
}
