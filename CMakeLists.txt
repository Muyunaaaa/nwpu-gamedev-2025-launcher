cmake_minimum_required(VERSION 3.30)
project(nwpu_gamedev_2025_launcher)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_PREFIX_PATH "D:/QT6.10/6.10.1/mingw_64")

# 1. 引入多媒体模块
find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        Multimedia
        MultimediaWidgets
        REQUIRED)

add_executable(nwpu_gamedev_2025_launcher
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        res.qrc
        UpdateToml.cpp
        settingswindow.cpp
        settingswindow.h
        settingswindow.ui
)

# 2. 链接多媒体库
target_link_libraries(nwpu_gamedev_2025_launcher
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
        Qt6::Multimedia
        Qt6::MultimediaWidgets
)

target_sources(nwpu_gamedev_2025_launcher PRIVATE app.rc)

# --- 部署逻辑修改 ---
if (WIN32 AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(DEBUG_SUFFIX)
    if (CMAKE_BUILD_TYPE MATCHES "Debug")
        set(DEBUG_SUFFIX "d")
    endif()

    set(QT_INSTALL_PATH "${CMAKE_PREFIX_PATH}")

    # 3. 更新拷贝列表，加入 Multimedia 相关 DLL
    foreach(QT_LIB Core Gui Widgets Multimedia MultimediaWidgets Network)
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "${QT_INSTALL_PATH}/bin/Qt6${QT_LIB}${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>")
    endforeach()

    # 拷贝平台插件 (platforms/qwindows.dll)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:${PROJECT_NAME}>/platforms")
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll"
            "$<TARGET_FILE_DIR:${PROJECT_NAME}>/platforms/")

    # 重点：视频播放通常需要 FFmpeg 插件 (Qt 6 默认后端)
    # 如果不拷贝 mediaservice 插件，程序可能运行但不报错，只是无法解码视频
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:${PROJECT_NAME}>/multimedia")
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${QT_INSTALL_PATH}/plugins/multimedia"
            "$<TARGET_FILE_DIR:${PROJECT_NAME}>/multimedia")
endif()