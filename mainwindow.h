#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QProcess>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QVideoSink>
#include <QVideoFrame>
#include <QImage>
#include <QPainter>
#include "settingswindow.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

    private slots:
        void on_closeWindow_clicked();
    void on_mini_clicked();
    void on_launch_clicked();
    void on_settings_clicked();
    void updatePlayerName(const QString& playerName);
    void updateServerIP(const QString& serverIP);
    void updateServerPort(int serverPort);
    void launch();

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override; // 核心：手动绘制视频帧

private:
    Ui::MainWindow *ui;
    QPoint m_dragPosition;
    QProcess* gameProcess = nullptr;
    SettingsWindow *sWindow = nullptr;

    // 视频播放相关组件 (移除了 QVideoWidget)
    QMediaPlayer *m_mediaPlayer = nullptr;
    QAudioOutput *m_audioOutput = nullptr;
    QVideoSink   *m_videoSink   = nullptr;
    QImage        m_currentFrame; // 存储当前视频帧

    void initVideoPlayer();
};

#endif // MAINWINDOW_H