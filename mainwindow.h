//
// Created by Administrator on 25-12-30.
//

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QProcess>


QT_BEGIN_NAMESPACE

namespace Ui {
    class MainWindow;
}

QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

    ~MainWindow() override;

private:
    Ui::MainWindow *ui;
    QPoint m_dragPosition;
    QProcess* gameProcess = nullptr;

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
};


#endif //MAINWINDOW_H
