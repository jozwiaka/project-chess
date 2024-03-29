cmake_minimum_required(VERSION 3.5)

project(App-Chess VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Network Core)  # Include Network and Core

set(PROJECT_SOURCES
    main.cpp
    chessview.cpp
    chessview.h
    chessview.ui
    chessmodel.h chessmodel.cpp
    chesscontroller.h chesscontroller.cpp
    chesssquare.h chesssquare.cpp
    cnnmodel.h cnnmodel.cpp
    chesspiece.h chesspiece.cpp
    chessboard.h chessboard.cpp
    chessgame.h chessgame.cpp
    promotiondialog.h promotiondialog.cpp
    endgamedialog.h endgamedialog.cpp
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(App-Chess
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
else()
    if(ANDROID)
        add_library(App-Chess SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(App-Chess
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(App-Chess PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Network Qt${QT_VERSION_MAJOR}::Core)  # Add Network and Core to target_link_libraries

if(${QT_VERSION} VERSION_LESS 6.1.0)
    set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.App-Chess)
endif()

set_target_properties(App-Chess PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS App-Chess
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(App-Chess)
endif()
