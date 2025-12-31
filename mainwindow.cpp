#include "mainwindow.h"
#include "ui_MainWindow.h"

#include <QMouseEvent>
#include <QMessageBox>
#include <QProcess>
#include <QCoreApplication>
#include <QDir>
#include <QTimer>

#include "settingswindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->logo->setAttribute(Qt::WA_TranslucentBackground);
    ui->logo->setStyleSheet("background: none;");
    ui->logo->setFrameShape(QFrame::NoFrame);

    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_dragPosition =
            event->globalPosition().toPoint() - frameGeometry().topLeft();
        event->accept();
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton) {
        move(event->globalPosition().toPoint() - m_dragPosition);
        event->accept();
    }
}

void MainWindow::on_launch_clicked()
{
    QString playerName = ui->editUserName->text().trimmed();
    QString host       = ui->editHost->text().trimmed();
    int serverPort     = ui->editPort->text().toInt();

    if (playerName.isEmpty()) {
        playerName = "default_player";
    }

    if (host.isEmpty()) {
        QMessageBox::warning(this, "错误", "请输入服务器地址");
        return;
    }

    try {
        updatePlayerName(playerName);
        updateServerIP(host);
        updateServerPort(serverPort);
    }

    catch (...) {
        QMessageBox::critical(this, "错误", "写入配置文件失败");
        return;
    }

    launch();
}

void MainWindow::on_settings_clicked()
{
    if (!sWindow) {
        sWindow = new SettingsWindow(this);
    }

    sWindow->adjustSize();

    sWindow->setWindowFlags(Qt::Window);

    int x = this->geometry().x() + (this->width() - sWindow->width()) / 2;
    int y = this->geometry().y() + (this->height() - sWindow->height()) / 2;

    sWindow->move(x, y);
    sWindow->show();
    sWindow->raise();
    sWindow->activateWindow();
}

void MainWindow::on_closeWindow_clicked()
{
    close();
}

void MainWindow::on_mini_clicked()
{
    showMinimized();
}

void MainWindow::launch() {
    if (!gameProcess) {
        gameProcess = new QProcess(this);
    }

    QString exePath =
        QCoreApplication::applicationDirPath() + "/moe-graphics.exe";

    gameProcess->start(exePath);

    if (!gameProcess->waitForStarted(3000)) {
        QMessageBox::critical(this, "错误", "无法启动游戏");
        return;
    }

    // 可选：隐藏 launcher，而不是销毁
    this->hide();
}

