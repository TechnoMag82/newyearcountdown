#include "mainwindow.h"
#include <QApplication>
#include <QVBoxLayout>
#include <QBitmap>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QCoreApplication::setOrganizationName("TechnoMag");
    QCoreApplication::setApplicationName("NewYearCountdown");
    QCoreApplication::setApplicationVersion("1.0");
    MainWindow mainWin;
    mainWin.show();
    return a.exec();
}
