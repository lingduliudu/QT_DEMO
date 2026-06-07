cmake_minimum_required(VERSION 3.16)

project(AutoDemo VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(APP_ICON_RESOURCE_WINDOWS "${CMAKE_CURRENT_SOURCE_DIR}/appicon.rc")

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Sql)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(AutoDemo
        ${APP_ICON_RESOURCE_WINDOWS}
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}



        editpath.h editpath.cpp editpath.ui
        dbdialog.h dbdialog.cpp dbdialog.ui
        detaildialog.h detaildialog.cpp detaildialog.ui
        tot_file.h
        tot_file.cpp
        inja.hpp json.hpp json_fwd.hpp
        throw.hpp
        environment.hpp
        config.hpp
        template.hpp
        node.hpp
        function_storage.hpp
        utils.hpp
        exceptions.hpp
        statistics.hpp
        parser.hpp
        lexer.hpp
        token.hpp
        renderer.hpp
        Toast.hpp
        resources.qrc






    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET AutoDemo APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(AutoDemo SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(AutoDemo
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(AutoDemo PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(AutoDemo PRIVATE Qt${QT_VERSION_MAJOR}::Sql)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.AutoDemo)
endif()
set_target_properties(AutoDemo PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS AutoDemo
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(AutoDemo)
endif()

# ==========================================
# 自动化部署：自动复制附属文件和文件夹
# ==========================================

# 1. 获取编译输出的 .exe 所在目标目录
set(TARGET_OUT_DIR "$<TARGET_FILE_DIR:${PROJECT_NAME}>")

# 2. 定义你需要复制的 4 个文件列表（假设它们都在项目源码根目录下）
set(DEPLOY_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/basedata.json"
    "${CMAKE_CURRENT_SOURCE_DIR}/dbconfig.json"
    "${CMAKE_CURRENT_SOURCE_DIR}/typemap.json"
    "${CMAKE_CURRENT_SOURCE_DIR}/detailconfig.json"
)

# 3. 定义需要复制的文件夹路径
set(SRC_TEMPLATE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/template")

# 4. 创建一个编译后的后续动作 (Post-Build)
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    # 动作 A：循环复制那 4 个独立的 JSON 文件
    COMMAND ${CMAKE_COMMAND} -E copy ${DEPLOY_FILES} "${TARGET_OUT_DIR}"

    # 动作 B：复制整个 template 文件夹到目标目录下的 template 文件夹
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${SRC_TEMPLATE_DIR}" "${TARGET_OUT_DIR}/template"

    COMMENT ">>> 正在自动复制 JSON 配置文件及 template 文件夹到输出目录..."
)
