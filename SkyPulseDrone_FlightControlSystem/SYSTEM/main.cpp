#include <QApplication>
#include <QVector>
#include "mainsystem.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qRegisterMetaType<QVector<quint8>>("QVector<quint8>");
    qRegisterMetaType<QVector<float>>("QVector<float>");

    /*Main Window Display*/
    MainSystem mainSystem;
    mainSystem.resize(300, 60);
    mainSystem.setWindowTitle("Main");
    mainSystem.show();  // Show main window
    return a.exec();
}
