#include <QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    // 方案 1：优先尝试硬件加速（对视频播放更流畅）
    QCoreApplication::setAttribute(Qt::AA_UseDesktopOpenGL);

    // 方案 2：如果还是黑屏，注释掉上面一行，开启下面这行
    // qputenv("QT_QUICK_BACKEND", "software");

    QApplication a(argc, argv);

    a.setWindowIcon(QIcon(":/icons/app.ico"));
    a.setQuitOnLastWindowClosed(false);

    MainWindow w;
    w.show();

    return a.exec();
}
