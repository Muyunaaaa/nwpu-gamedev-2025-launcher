#include "settingswindow.h"
#include "ui_SettingsWindow.h"
#include "tomlplusplus/toml.hpp"
#include <QPainter>
#include <QColor>
#include <fstream>
#include <QDebug>

SettingsWindow::SettingsWindow(QWidget *parent) :
    QWidget(parent), ui(new Ui::SettingsWindow) {

    ui->setupUi(this);

    // this->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    // this->setAttribute(Qt::WA_TranslucentBackground);

    // 设置样式表 (包含新增的 resolutionBox 样式)
    this->setStyleSheet(
        "QWidget#SettingsWindow { background-color: rgba(255, 255, 255, 180); border-radius: 20px; border: 1px solid rgba(0,0,0,40); }"
        "QLabel { color: #333333; font-weight: bold; background: transparent; }"
        "QSlider::groove:horizontal { border: 1px solid #BBB; height: 6px; background: rgba(0,0,0,10); border-radius: 3px; }"
        "QSlider::handle:horizontal { background: #0078D7; border: 2px solid white; width: 18px; height: 18px; margin: -6px 0; border-radius: 9px; }"
        "QComboBox { border: 1px solid #CCC; border-radius: 4px; padding: 2px 10px; background: white; }"
        "QPushButton { background: white; border: 1px solid #CCC; border-radius: 6px; padding: 5px; }"
        "QPushButton:hover { background: #0078D7; color: white; }"
    );

    // 信号槽连接
    connect(ui->horizontalSlider_R, &QSlider::valueChanged, this, &SettingsWindow::updateCrosshairColor);
    connect(ui->horizontalSlider_G, &QSlider::valueChanged, this, &SettingsWindow::updateCrosshairColor);
    connect(ui->horizontalSlider_B, &QSlider::valueChanged, this, &SettingsWindow::updateCrosshairColor);
    connect(ui->horizontalSlider_A, &QSlider::valueChanged, this, &SettingsWindow::updateCrosshairColor);

    // 关键：连接分辨率切换信号
    connect(ui->resolutionBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &SettingsWindow::on_resolutionBox_currentIndexChanged);

    loadSettings();
    updateCrosshairColor();
}

SettingsWindow::~SettingsWindow() {
    delete ui;
}

void SettingsWindow::loadSettings() {
    try {
        auto tbl = toml::parse_file("settings.toml");

        // 1. 还原分辨率选项 [window]
        int w = tbl["window"]["width"].value_or(1920);
        int h = tbl["window"]["height"].value_or(1080);

        // 根据文件数值自动选中 ComboBox 项目
        if (w == 1920 && h == 1080) ui->resolutionBox->setCurrentIndex(0);
        else if (w == 1280 && h == 720) ui->resolutionBox->setCurrentIndex(1);

        // 2. 还原灵敏度 [player]
        double sens = tbl["player"]["rotation_speed"].value_or(0.1);
        ui->sensitiveSlider->setValue(qRound(sens * 100.0));
        ui->sensitiveLabel->setText(QString::number(sens, 'f', 2));

        // 3. 还原颜色 [crosshair]
        if (auto* colorArr = tbl["crosshair"]["color"].as_array()) {
            ui->horizontalSlider_R->setValue(colorArr->get(0)->value_or(1.0) * 100);
            ui->horizontalSlider_G->setValue(colorArr->get(1)->value_or(1.0) * 100);
            ui->horizontalSlider_B->setValue(colorArr->get(2)->value_or(1.0) * 100);
            ui->horizontalSlider_A->setValue(colorArr->get(3)->value_or(1.0) * 100);
        }
    } catch (...) { qDebug() << "Failed to load settings.toml"; }
}

// 修改分辨率并保存到 [window]
void SettingsWindow::on_resolutionBox_currentIndexChanged(int index) {
    int width = 1920;
    int height = 1080;

    if (index == 1) { // 720P
        width = 1280;
        height = 720;
    }

    try {
        auto tbl = toml::parse_file("settings.toml");
        tbl["window"].as_table()->insert_or_assign("width", width);
        tbl["window"].as_table()->insert_or_assign("height", height);

        std::ofstream ofs("settings.toml", std::ios::trunc);
        ofs << tbl;
    } catch (...) { qDebug() << "Failed to save resolution to toml"; }
}

void SettingsWindow::updateCrosshairColor() {
    QPixmap pixmap(":/images/crosshair.png");
    if (pixmap.isNull()) return;

    double r_f = ui->horizontalSlider_R->value() / 100.0;
    double g_f = ui->horizontalSlider_G->value() / 100.0;
    double b_f = ui->horizontalSlider_B->value() / 100.0;
    double a_f = ui->horizontalSlider_A->value() / 100.0;

    QPixmap result(pixmap.size());
    result.fill(Qt::transparent);
    QPainter painter(&result);
    painter.drawPixmap(0, 0, pixmap);
    painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
    painter.fillRect(result.rect(), QColor::fromRgbF(r_f, g_f, b_f, a_f));
    painter.end();
    ui->label_crosshair->setPixmap(result);

    try {
        auto tbl = toml::parse_file("settings.toml");
        tbl["crosshair"].as_table()->insert_or_assign("color", toml::array{ r_f, g_f, b_f, a_f });
        std::ofstream ofs("settings.toml", std::ios::trunc);
        ofs << tbl;
    } catch (...) {}
}

void SettingsWindow::on_sensitiveSlider_valueChanged(int value) {
    double sens = value / 100.0;
    ui->sensitiveLabel->setText(QString::number(sens, 'f', 2));
    try {
        auto tbl = toml::parse_file("settings.toml");
        tbl["player"].as_table()->insert_or_assign("rotation_speed", sens);
        std::ofstream ofs("settings.toml", std::ios::trunc);
        ofs << tbl;
    } catch (...) {}
}

void SettingsWindow::on_back_clicked() { this->close(); }
void SettingsWindow::on_confirm_clicked() { this->close(); }

void SettingsWindow::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        m_dragPosition = event->globalPosition().toPoint() - frameGeometry().topLeft();
        event->accept();
    }
}

void SettingsWindow::mouseMoveEvent(QMouseEvent *event) {
    if (event->buttons() & Qt::LeftButton) {
        move(event->globalPosition().toPoint() - m_dragPosition);
        event->accept();
    }
}