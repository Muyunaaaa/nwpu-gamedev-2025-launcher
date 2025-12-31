#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H

#include <QWidget>
#include <QPoint>
#include <QMouseEvent>

namespace Ui { class SettingsWindow; }

class SettingsWindow : public QWidget {
    Q_OBJECT

public:
    explicit SettingsWindow(QWidget *parent = nullptr);
    ~SettingsWindow() override;

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

    private slots:
        void updateCrosshairColor();
    void on_sensitiveSlider_valueChanged(int value);
    void on_resolutionBox_currentIndexChanged(int index); // 新增：处理分辨率切换
    void on_back_clicked();
    void on_confirm_clicked();

private:
    Ui::SettingsWindow *ui;
    QPoint m_dragPosition;

    void loadSettings();
};

#endif // SETTINGSWINDOW_H