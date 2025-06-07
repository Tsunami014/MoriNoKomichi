cmake_minimum_required(VERSION 3.5)

project(MoriNoKomichi VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Svg)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Svg)

set(PROJECT_SOURCES
    main.cpp
    window.cpp window.h

    taskloading.h taskloading.cpp

    sections/sections.cpp sections/sections.h
    sections/tasksview.cpp
    sections/newoverlay.cpp
    sections/taskoverlay.cpp
)
set(PROJECT_WIDGETS
    sections/widgets/taskwidget.h sections/widgets/taskwidget.cpp
    sections/widgets/svgbtnwidget.h sections/widgets/svgbtnwidget.cpp
    sections/widgets/graphicsviewcanvas.h sections/widgets/graphicsviewcanvas.cpp
    sections/widgets/bigtaskwidget.h sections/widgets/bigtaskwidget.cpp
)
set(PROJECT_TOOLS
    sections/widgets/tools/drawtools.h sections/widgets/tools/drawtools.cpp
    sections/widgets/tools/texttools.h sections/widgets/tools/texttools.cpp
)
set(PROJECT_RESOURCES
    resources.qrc
)

if(ANDROID)
    add_library(${PROJECT_NAME} SHARED
        ${PROJECT_SOURCES}
        ${PROJECT_WIDGETS}
        ${PROJECT_RESOURCES}
        ${PROJECT_TOOLS}
    )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
else()
    add_executable(${PROJECT_NAME}
        ${PROJECT_SOURCES}
        ${PROJECT_WIDGETS}
        ${PROJECT_RESOURCES}
        ${PROJECT_TOOLS}
    )
endif()

target_link_libraries(${PROJECT_NAME} PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Svg)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.MoriNoKomichi)
endif()
set_target_properties(${PROJECT_NAME} PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS ${PROJECT_NAME}
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(${PROJECT_NAME})
endif()
