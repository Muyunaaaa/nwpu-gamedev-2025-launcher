#include "mainwindow.h"
#include "ui_MainWindow.h"
#include <QMouseEvent>
#include <QMessageBox>
#include <QCoreApplication>
#include <QUrl>
#include <QPainter>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 1. 窗口基础设置
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground);

    // 2. 确保 UI 层透明，不遮挡 paintEvent 画出来的背景
    ui->centralwidget->setAttribute(Qt::WA_TranslucentBackground);
    ui->centralwidget->setStyleSheet("background: transparent;");

    initVideoPlayer();
}

MainWindow::~MainWindow()
{
    if (m_mediaPlayer) m_mediaPlayer->stop();
    delete ui;
}

void MainWindow::initVideoPlayer()
{
    m_mediaPlayer = new QMediaPlayer(this);
    m_audioOutput = new QAudioOutput(this);
    m_videoSink   = new QVideoSink(this);

    m_mediaPlayer->setAudioOutput(m_audioOutput);
    m_mediaPlayer->setVideoOutput(m_videoSink); // 视频输出给 Sink 而不是 Widget

    // 核心逻辑：每当视频帧变化时，将其转换为 Image 并刷新界面
    connect(m_videoSink, &QVideoSink::videoFrameChanged, this, [this](const QVideoFrame &frame){
        if (frame.isValid()) {
            m_currentFrame = frame.toImage();
            this->update(); // 触发 paintEvent
        }
    });

    m_audioOutput->setVolume(0.1);
    m_mediaPlayer->setLoops(QMediaPlayer::Infinite);

    QString videoPath = QCoreApplication::applicationDirPath() + "/bg.mp4";
    m_mediaPlayer->setSource(QUrl::fromLocalFile(videoPath));
    m_mediaPlayer->play();
}

// 核心：手动把视频帧画在最底层
void MainWindow::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    // 如果有有效的视频帧，则将其作为背景绘制
    if (!m_currentFrame.isNull()) {
        painter.drawImage(this->rect(), m_currentFrame);
    } else {
        // 视频未加载时，画个黑底，防止窗口完全透明看不见
        painter.fillRect(this->rect(), QColor(30, 30, 30));
    }

    // 继续执行默认的绘制逻辑（绘制按钮、输入框等子控件）
    QMainWindow::paintEvent(event);
}

// --- 窗口交互逻辑 (保持不变) ---

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_dragPosition = event->globalPosition().toPoint() - frameGeometry().topLeft();
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
    QString portStr    = ui->editPort->text().trimmed();
    int serverPort     = portStr.toInt();

    // 默认值处理
    if (playerName.isEmpty()) playerName = "default_player";
    if (host.isEmpty())       host = "nwpua335.online";
    if (serverPort == 0)      serverPort = 14532;

    try {
        // 这里调用你原有的 UpdateToml 逻辑
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

void MainWindow::launch() {
    if (!gameProcess) {
        gameProcess = new QProcess(this);
    }

    QString exePath = QCoreApplication::applicationDirPath() + "/moe-graphics.exe";

    gameProcess->start(exePath);

    if (!gameProcess->waitForStarted(3000)) {
        QMessageBox::critical(this, "错误", "无法启动游戏，请检查游戏文件是否存在。");
        return;
    }

    // 启动成功后隐藏启动器
    this->hide();

    // 游戏进程结束后可以考虑重新显示启动器
    connect(gameProcess, &QProcess::finished, this, &MainWindow::show);
}

void MainWindow::on_settings_clicked()
{
    if (!sWindow) {
        sWindow = new SettingsWindow(this);
    }
    sWindow->setWindowFlags(Qt::Window);

    // 居中显示设置窗口
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