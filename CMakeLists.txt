cmake_minimum_required(VERSION 3.20)
project(ExpressManager)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
if(WIN32)
    set(CMAKE_PREFIX_PATH "D:/Qt/6.9.0/mingw_64")  # Windows Qt 安装路径，macOS 时可在命令行指定
endif ()
if(APPLE)
    set(CMAKE_PREFIX_PATH "/Users/littleqiu/Qt/6.9.0/macos")
endif ()

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Sql)

file(GLOB MODEL_SRC models/*.cpp models/*.h)
file(GLOB CONTROLLER_SRC controllers/*.cpp controllers/*.h)
file(GLOB VIEW_SRC views/*.cpp views/*.h)
file(GLOB DB_SRC db/*.cpp db/*.h)
file(GLOB UI_FILES views/*.ui)

add_executable(ExpressManager
        main.cpp
        ${MODEL_SRC}
        ${CONTROLLER_SRC}
        ${VIEW_SRC}
        ${DB_SRC}
        ${UI_FILES}
)

target_include_directories(ExpressManager PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/models
        ${CMAKE_CURRENT_SOURCE_DIR}/controllers
        ${CMAKE_CURRENT_SOURCE_DIR}/views
        ${CMAKE_CURRENT_SOURCE_DIR}/db
)

target_link_libraries(ExpressManager
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
        Qt6::Sql
)

# === 平台适配 ===
if (WIN32 AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(DEBUG_SUFFIX "")
    if (MSVC AND CMAKE_BUILD_TYPE MATCHES "Debug")
        set(DEBUG_SUFFIX "d")
    endif()

    set(QT_INSTALL_PATH "${CMAKE_PREFIX_PATH}")
    if(NOT EXISTS "${QT_INSTALL_PATH}/bin")
        set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
        if(NOT EXISTS "${QT_INSTALL_PATH}/bin")
            set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
        endif()
    endif()

    # 拷贝 Windows 平台插件
    if(EXISTS "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E make_directory
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/"
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
    endif()

    # 拷贝 Qt6 核心库 DLL
    foreach(QT_LIB Core Gui Widgets Sql)
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/bin/Qt6${QT_LIB}${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>")
    endforeach()

    # 拷贝 SQLite 驱动
    if(EXISTS "${QT_INSTALL_PATH}/plugins/sqldrivers/qsqlite${DEBUG_SUFFIX}.dll")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E make_directory
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/sqldrivers/"
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/plugins/sqldrivers/qsqlite${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/sqldrivers/")
    endif()
endif()

if(APPLE)
    set(QT_PLUGIN_PLATFORM_PATH "${CMAKE_PREFIX_PATH}/plugins/platforms")
    set(PLATFORM_PLUGIN "libqcocoa.dylib")

#    # 拷贝 macOS 平台插件（可选，推荐用 macdeployqt 替代）
#    if(EXISTS "${QT_PLUGIN_PLATFORM_PATH}/${PLATFORM_PLUGIN}")
#        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
#                COMMAND ${CMAKE_COMMAND} -E make_directory
#                "$<TARGET_BUNDLE_DIR:${PROJECT_NAME}>/Contents/plugins/platforms/"
#                COMMAND ${CMAKE_COMMAND} -E copy
#                "${QT_PLUGIN_PLATFORM_PATH}/${PLATFORM_PLUGIN}"
#                "$<TARGET_BUNDLE_DIR:${PROJECT_NAME}>/Contents/plugins/platforms/")
#    endif()
endif()