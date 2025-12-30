#include "mainwindow.h"

#include "tomlplusplus/toml.hpp"
#include <fstream>
#include <QString>

// 修改玩家名
void MainWindow::updatePlayerName(const QString& playerName)
{
    const std::string path = "settings.toml";
    toml::table tbl = toml::parse_file(path);

    tbl["gameplay"].as_table()->insert_or_assign(
        "player_name",
        playerName.toStdString()
    );

    std::ofstream ofs(path, std::ios::trunc);
    ofs << tbl;
}

// 修改服务器 IP
void MainWindow::updateServerIP(const QString& serverIP)
{
    const std::string path = "settings.toml";
    toml::table tbl = toml::parse_file(path);

    tbl["network"].as_table()->insert_or_assign(
        "server_address",
        serverIP.toStdString()
    );

    std::ofstream ofs(path, std::ios::trunc);
    ofs << tbl;
}

// 修改服务器端口
void MainWindow::updateServerPort(int serverPort)
{
    const std::string path = "settings.toml";
    toml::table tbl = toml::parse_file(path);

    tbl["network"].as_table()->insert_or_assign(
        "server_port",
        serverPort
    );

    std::ofstream ofs(path, std::ios::trunc);
    ofs << tbl;
}
