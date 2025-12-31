#include <QApplication>
#include <QPushButton>

#include "mainwindow.h"

int main(int argc, char *argv[])
{
    qputenv("QT_QPA_PLATFORM", "windows");
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon(":/icons/app.ico"));
    a.setQuitOnLastWindowClosed(false);
    MainWindow w;
    w.show();
    return a.exec();
}
