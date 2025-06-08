cmake_minimum_required(VERSION 3.20)
project(CPPCourseDesign)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)  # 如果你用了信号槽，这一行必须保留
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_PREFIX_PATH "D:/Qt/6.9.0/mingw_64")

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Sql)

file(GLOB MODEL_SRC models/*.cpp models/*.h)
file(GLOB CONTROLLER_SRC controllers/*.cpp controllers/*.h)
file(GLOB VIEW_SRC views/*.cpp views/*.h)
file(GLOB DB_SRC db/*.cpp db/*.h)
file(GLOB UI_FILES views/*.ui)

add_executable(CPPCourseDesign
    main.cpp
    ${MODEL_SRC}
    ${CONTROLLER_SRC}
    ${VIEW_SRC}
    ${DB_SRC}
    ${UI_FILES}
)

target_link_libraries(CPPCourseDesign
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
        Qt6::Sql
)
if(EXISTS "${CMAKE_PREFIX_PATH}/plugins/sqldrivers/qsqlite${DEBUG_SUFFIX}.dll")
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E make_directory
            "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/sqldrivers/")
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy
            "${CMAKE_PREFIX_PATH}/plugins/sqldrivers/qsqlite${DEBUG_SUFFIX}.dll"
            "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/sqldrivers/")
endif()
target_include_directories(CPPCourseDesign PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/models
    ${CMAKE_CURRENT_SOURCE_DIR}/controllers
    ${CMAKE_CURRENT_SOURCE_DIR}/views
    ${CMAKE_CURRENT_SOURCE_DIR}/db
)

if (WIN32 AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(DEBUG_SUFFIX)
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
    if(EXISTS "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E make_directory
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
    endif()
    foreach(QT_LIB Core Gui Widgets Sql)
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/bin/Qt6${QT_LIB}${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>")
    endforeach()
endif()
